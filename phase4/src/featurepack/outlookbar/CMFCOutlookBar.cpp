// CMFCOutlookBar — OpenMFC implementation.
// Sources: global_cmfcoutlookbar.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCOutlookBarSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CMemDCSupport.h"

// CPaintDC placement thunks (core/gdi/CPaintDC.cpp).
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// Visual-manager background-fill thunk (featurepack/visualmanager/Thunks.cpp)
// and its exported static pointer mirror (core/runtime/StaticData.cpp).
extern "C" void MS_ABI impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CBasePane* pBar, CRect rect, CRect rectClip, int bNCArea);
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;

// The shadow reserves the 0x568-byte retail object size but models only the
// fields used here. The retail constructor writes m_bMode2003 at +0x550
// (mfc140u RVA 0x96b90); m_hWnd is the inherited CWnd field at +0x40.
static_assert(sizeof(OutlookBar) == 0x568, "CMFCOutlookBar size mismatch");
static_assert(offsetof(OutlookBar, m_hWnd) == 0x40, "CMFCOutlookBar::m_hWnd offset");
static_assert(offsetof(OutlookBar, m_bMode2003) == 0x550, "CMFCOutlookBar::m_bMode2003 offset");

// Retail mfc140.dll (ANSI twin) RVA 0x98530: when this is non-null and
// m_hWnd is null, store bMode in m_bMode2003. The retail NULL-this path faults
// on the store; this existing body guards it instead.
// Symbol: ?SetMode2003@CMFCOutlookBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetMode2003_CMFCOutlookBar__QEAAXH_Z(void* pThis, int bMode2003)
{
    if (pThis == nullptr)
        return;

    OutlookBar* pBar = static_cast<OutlookBar*>(pThis);
    if (pBar->m_hWnd == nullptr) {
        pBar->m_bMode2003 = bMode2003;
    }
}
// Stub: retail construction calls the CBaseTabbedPane constructor and
// initializes CMFCOutlookBar fields (mfc140u RVA 0x96b90); this thunk only
// returns pThis because no compatible derived OutlookBar vtable is modeled.
// Symbol: ??0CMFCOutlookBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCOutlookBar__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1CMFCOutlookBar@@UEAA@XZ
// Stub: retail cleans up members at +0x518, +0x530 and +0x560 before its base
// destructor (mfc140u RVA 0x96c90); those members are absent from this shadow.
extern "C" void MS_ABI impl___1CMFCOutlookBar__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CanAcceptPane@CMFCOutlookBar@@UEBAHPEBVCBasePane@@@Z
// Stub: retail performs runtime-class tests and calls this object's virtual
// slots +0x3a8, +0x2e0 and +0x360 (mfc140u RVA 0x97010). This shadow has no
// modeled OutlookBar vtable/slot behavior for those calls.
extern "C" int MS_ABI impl__CanAcceptPane_CMFCOutlookBar__UEBAHPEBVCBasePane___Z(
    const void* pThis, const void* pPane) {
    (void)pThis;
    (void)pPane;
    return 0;
}

// Symbol: ?Create@CMFCOutlookBar@@UEAAHPEB_WPEAVCWnd@@AEBUtagRECT@@IKKPEAUCCreateContext@@@Z
// Stub: retail first delegates pane creation, then configures docking/tab
// behavior (mfc140u RVA 0x96f20); the derived state and complete base path are
// not represented by the available shadow/thunks.
extern "C" int MS_ABI impl__Create_CMFCOutlookBar__UEAAHPEB_WPEAVCWnd__AEBUtagRECT__IKKPEAUCCreateContext___Z(
    void* pThis, const wchar_t* caption, void* pParentWnd, const RECT* pRect,
    unsigned int nID, unsigned long dwStyle, unsigned long dwControlBarStyle,
    void* pContext) {
    (void)pThis;
    (void)caption;
    (void)pParentWnd;
    (void)pRect;
    (void)nID;
    (void)dwStyle;
    (void)dwControlBarStyle;
    (void)pContext;
    return 0;
}

// Symbol: ?CreateCustomPage@CMFCOutlookBar@@QEAAPEAVCMFCOutlookBarPane@@PEB_WHKH@Z
// Stub: retail allocates/creates a page, attaches it to tab and custom-page
// collections, and updates a global page-ID bitmap (mfc140u RVA 0x97250); that
// collection/child-window infrastructure is unavailable in this shadow.
extern "C" void* MS_ABI impl__CreateCustomPage_CMFCOutlookBar__QEAAPEAVCMFCOutlookBarPane__PEB_WHKH_Z(
    void* pThis, const wchar_t* pageName, int bActivatePage,
    unsigned long dwEnabledDocking, int bEnableTextLabels) {
    (void)pThis;
    (void)pageName;
    (void)bActivatePage;
    (void)dwEnabledDocking;
    (void)bEnableTextLabels;
    return nullptr;
}

// Symbol: ?CreateObject@CMFCOutlookBar@@SAPEAVCObject@@XZ
// Stub: retail allocates a 0x568-byte object and invokes the constructor
// (mfc140u RVA 0x96b20). The constructor body (RVA 0x96b90, mfc140u) calls the
// CBaseTabbedPane constructor and initializes derived fields, but this file's
// constructor thunk is only an identity return and no compatible OutlookBar
// vtable is modeled; returning an object here would not provide safe dispatch.
extern "C" CObject* MS_ABI impl__CreateObject_CMFCOutlookBar__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?FindAvailablePageID@CMFCOutlookBar@@IEAAIXZ
// Retail scans a 100-byte page-ID bitmap and returns 0xf000 plus the first
// free index, or 0xffff (mfc140u RVA 0x97f40). This local bitmap models that
// state; the still-stubbed create/remove paths do not yet update it.
namespace {
unsigned char g_outlookBarPageIdUsed[100] = {};
}
extern "C" unsigned int MS_ABI impl__FindAvailablePageID_CMFCOutlookBar__IEAAIXZ(
    void* pThis) {
    (void)pThis; // The retail implementation does not read this.
    for (unsigned int i = 0; i < 100; ++i) {
        if (g_outlookBarPageIdUsed[i] == 0)
            return 0xf000u + i;
    }
    return 0xffffu;
}

// Symbol: ?FloatTab@CMFCOutlookBar@@UEAAHPEAVCWnd@@HW4AFX_DOCK_METHOD@@H@Z
// Stub: retail obtains the tab-control pointer through vtable byte offset
// +0x750, queries that control at byte offset +0x368, and calls
// CBaseTabbedPane::FloatTab only when the query returns >1 (mfc140u entries
// RVA 0x97fc0 for CMFCOutlookBar and RVA 0x13950 for CBaseTabbedPane). This
// class has no modeled compatible virtual dispatch here, and the available
// base FloatTab thunk is itself a stub.
extern "C" int MS_ABI impl__FloatTab_CMFCOutlookBar__UEAAHPEAVCWnd__HW4AFX_DOCK_METHOD__H_Z(
    void* pThis, void* pWnd, int nTabID, int dockMethod, int bHide) {
    (void)pThis;
    (void)pWnd;
    (void)nTabID;
    (void)dockMethod;
    (void)bHide;
    return 0;
}

// Symbol: ?GetDockingStatus@CMFCOutlookBar@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@H@Z
// Stub: retail uses the tab window at +0x4e8 and several virtual docking
// queries (mfc140u RVA 0x970e0); that member and the dependent layout are absent.
extern "C" int MS_ABI impl__GetDockingStatus_CMFCOutlookBar__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(
    const void* pThis, long long point, int nSensitivity) {
    (void)pThis;
    (void)point;
    (void)nSensitivity;
    return 0;
}

// Symbol: ?GetTabArea@CMFCOutlookBar@@UEBAXAEAVCRect@@0@Z
// Stub: retail initializes both output rectangles and delegates the tab-area
// calculation to the child at +0x4e8 in one branch (mfc140u RVA 0x96d50); that
// child member is not represented in the shadow.
extern "C" void MS_ABI impl__GetTabArea_CMFCOutlookBar__UEBAXAEAVCRect__0_Z(
    const void* pThis, CRect& rectTabAreaTop, CRect& rectTabAreaBottom) {
    (void)pThis;
    (void)rectTabAreaTop;
    (void)rectTabAreaBottom;
}

// Symbol: ?LoadState@CMFCOutlookBar@@UEAAHPEB_WHI@Z
// Stub: retail parses archived custom-page state and creates/configures page
// windows (mfc140u RVA 0x97530); the required archive and page infrastructure is
// not available through verified thunks here.
extern "C" int MS_ABI impl__LoadState_CMFCOutlookBar__UEAAHPEB_WHI_Z(
    void* pThis, const wchar_t* profileName, int nIndex, unsigned int uiID) {
    (void)pThis;
    (void)profileName;
    (void)nIndex;
    (void)uiID;
    return 0;
}

// Symbol: ?OnCreate@CMFCOutlookBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Stub: retail creates the child tab control at +0x4e8 and initializes its
// docking/layout state (mfc140u RVA 0x96de0); that child layout is not modeled.
extern "C" int MS_ABI impl__OnCreate_CMFCOutlookBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return 0;
}

// Symbol: ?OnPaint@CMFCOutlookBar@@IEAAXXZ
// The mfc140u map did not resolve this symbol. The ANSI twin resolves the
// export at entry RVA 0xaa30 (mfc140.dll); its body is shared with
// CAutoHideDockSite::OnPaint and is transcribed here. This uses the exported
// visual-manager pointer mirror instead of retail's GetInstance helper, so it
// does not lazily create a manager; the NULL-this guard is also an OpenMFC
// safety deviation.
extern "C" void MS_ABI impl__OnPaint_CMFCOutlookBar__IEAAXXZ(void* pThis) {
    if (pThis == nullptr)
        return;

    alignas(CPaintDC) unsigned char paintDCStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(paintDCStorage);
    CWnd* pWnd = reinterpret_cast<CWnd*>(pThis);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, pWnd);

    alignas(S_Cmemdc) unsigned char memDCStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memDCStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, pPaintDC, pWnd);
    CDC* pDC = pMemDC->m_bMemDC
        ? reinterpret_cast<CDC*>(&pMemDC->m_dcMem)
        : static_cast<CDC*>(pMemDC->m_dc);

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(static_cast<OutlookBar*>(pThis)->m_hWnd, &rectClient);
    CMFCVisualManager* pVisualManager = static_cast<CMFCVisualManager*>(
        impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    if (pVisualManager != nullptr) {
        impl__OnFillBarBackground_CMFCVisualManager__UEAAXPEAVCDC__PEAVCBasePane__VCRect__2H_Z(
            pVisualManager, pDC, reinterpret_cast<CBasePane*>(pThis),
            CRect(rectClient), CRect(rectClient), FALSE);
    }

    impl___1CMemDC__UEAA_XZ(pMemDC);
    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}

// Symbol: ?RemoveCustomPage@CMFCOutlookBar@@QEAAHIPEAVCMFCOutlookBarTabCtrl@@@Z
// Stub: the mfc140u map did not resolve this symbol, but the ANSI twin resolves
// it at RVA 0x979c0 (mfc140.dll). The body checks tab/page runtime classes,
// searches the custom-page list at +0x518, updates the page-ID bitmap, and
// removes the page; this file has no modeled list or tab-control state.
extern "C" int MS_ABI impl__RemoveCustomPage_CMFCOutlookBar__QEAAHIPEAVCMFCOutlookBarTabCtrl___Z(
    void* pThis, unsigned int uiPage, void* pTargetWnd) {
    (void)pThis;
    (void)uiPage;
    (void)pTargetWnd;
    return 0;
}

// Symbol: ?SaveState@CMFCOutlookBar@@UEAAHPEB_WHI@Z
// Stub: the mfc140u map did not resolve this UTF-16 signature. The ANSI twin's
// counterpart (?SaveState@CMFCOutlookBar@@UEAAHPEBDHI@Z) is at RVA 0x98060
// (mfc140.dll) and serializes custom-page state through archive helpers; that
// archive/list infrastructure is not modeled by the verified support here.
extern "C" int MS_ABI impl__SaveState_CMFCOutlookBar__UEAAHPEB_WHI_Z(
    void* pThis, const wchar_t* profileName, int nIndex, unsigned int uiID) {
    (void)pThis;
    (void)profileName;
    (void)nIndex;
    (void)uiID;
    return 0;
}

// Symbol: ?SetButtonsFont@CMFCOutlookBar@@QEAAXPEAVCFont@@H@Z
// Stub: the mfc140u map did not resolve this symbol, but the ANSI twin resolves
// it at RVA 0x98550 (mfc140.dll): it stores pFont at +0x558 and, when redraw is
// requested and m_hWnd is non-null, calls RedrawWindow(hwnd, nullptr, 0, 0x585).
// The current shadow has no member for +0x558; do not model it via a guessed
// field or access until the owned shadow header is extended.
extern "C" void MS_ABI impl__SetButtonsFont_CMFCOutlookBar__QEAAXPEAVCFont__H_Z(
    void* pThis, CFont* pFont, int bRedraw) {
    (void)pThis;
    (void)pFont;
    (void)bRedraw;
}

