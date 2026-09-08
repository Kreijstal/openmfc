// CBasePane — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?CreateEx@CBasePane@@UEAAHKPEB_W0KAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__CreateEx_CBasePane__UEAAHKPEB_W0KAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CBasePane* pThis, unsigned long dwStyleEx, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID, unsigned long, CCreateContext* pContext) {
    (void)dwStyleEx;
    (void)lpszWindowName;
    if (pThis == nullptr) return FALSE;
    CRect useRect = NormalizeRect(&rect);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        PaneCoreState& state = g_paneCoreState_Cbarcore[pThis];
        state.parent = pParentWnd;
        state.recentRect = useRect;
        state.visible = (dwStyle & WS_VISIBLE) ? TRUE : FALSE;
        if (state.minSize.cx <= 0) state.minSize.cx = 32;
        if (state.minSize.cy <= 0) state.minSize.cy = 32;
    }
    return TryCreatePaneWindow(static_cast<CWnd*>(pThis), lpszClassName, dwStyle, useRect, pParentWnd, nID, pContext);
}
// Symbol: ?ShowPane@CBasePane@@UEAAXHHH@Z
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int, int) {
    if (pThis == nullptr) return;
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Cbarcore);
        g_paneCoreState_Cbarcore[pThis].visible = bShow ? TRUE : FALSE;
    }
    if (pThis->GetSafeHwnd() != nullptr) {
        pThis->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
    }
}
// Symbol: ?GetRuntimeClass@CBasePane@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBasePane__UEBAPEAUCRuntimeClass__XZ(const CBasePane*) {
    return CBasePane::GetThisClass();
}
// Symbol: ?GetThisClass@CBasePane@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBasePane__SAPEAUCRuntimeClass__XZ() {
    return CBasePane::GetThisClass();
}
CBasePane::CBasePane() {
    // Zero the retail member block (CWnd subobject is constructed already),
    // then apply the non-zero retail defaults.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(CBasePane) - sizeof(CWnd));
    new (&m_AccData) CAccessibilityData();
    m_bEnableIDChecking = TRUE;
    m_dwEnabledAlignment = CBRS_ALIGN_ANY;
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = FALSE;
    state.canAutoHide = FALSE;
    state.hasGripper = FALSE;
}
CBasePane::~CBasePane() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore.erase(this);
}
BOOL CBasePane::CanAcceptPane(const CBasePane*) const { return TRUE; }
BOOL CBasePane::CanAutoHide() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canAutoHide : FALSE;
}
BOOL CBasePane::CanBeAttached() const { return TRUE; }
BOOL CBasePane::CanBeClosed() const { return TRUE; }
BOOL CBasePane::CanBeDocked() const { return TRUE; }
BOOL CBasePane::CanBeFloating() const { return CanFloat(); }
BOOL CBasePane::CanBeResized() const { return TRUE; }
BOOL CBasePane::CanFloat() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.canFloat : FALSE;
}
BOOL CBasePane::DoesAllowSiblingBars() const { return TRUE; }
BOOL CBasePane::HasGripper() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.hasGripper : FALSE;
}
BOOL CBasePane::IsAccessibilityCompatible() { return TRUE; }
void CBasePane::SetPaneAlignment(DWORD dwAlignment) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    g_paneCoreState_Mfccore[this].alignment = dwAlignment;
}
DWORD CBasePane::GetPaneAlignment() const {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    auto it = g_paneCoreState_Mfccore.find(this);
    return it != g_paneCoreState_Mfccore.end() ? it->second.alignment : 0;
}
void* CBasePane::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags, void* pExtra) {
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        PaneCoreState& state = g_paneCoreState_Mfccore[this];
        CRect rect = state.recentRect;
        if ((nFlags & SWP_NOMOVE) == 0) {
            rect.left = x;
            rect.top = y;
        }
        if ((nFlags & SWP_NOSIZE) == 0) {
            rect.right = rect.left + std::max(0, cx);
            rect.bottom = rect.top + std::max(0, cy);
        }
        state.recentRect = rect;
    }
    HWND hwnd = GetSafeHwnd();
    if (hwnd != nullptr) {
        HWND hwndInsertAfter = pWndInsertAfter != nullptr ? pWndInsertAfter->GetSafeHwnd() : nullptr;
        ::SetWindowPos(hwnd, hwndInsertAfter, x, y, cx, cy, nFlags);
    }
    return pExtra;
}
void CBasePane::CalcFixedLayout(BOOL bStretch, BOOL bHorz) {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];

    if (state.minSize.cx <= 0) state.minSize.cx = bHorz ? 120 : 64;
    if (state.minSize.cy <= 0) state.minSize.cy = bHorz ? 64 : 120;

    if (state.recentRect.Width() <= 0 || state.recentRect.Height() <= 0) {
        state.recentRect = CRect(0, 0, state.minSize.cx, state.minSize.cy);
    }

    if (bStretch) {
        if (bHorz) {
            if (state.recentRect.Height() < state.minSize.cy) {
                state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
            }
            if (state.recentRect.Width() < state.minSize.cx) {
                state.recentRect.right = state.recentRect.left + state.minSize.cx;
            }
        } else {
            if (state.recentRect.Width() < state.minSize.cx) {
                state.recentRect.right = state.recentRect.left + state.minSize.cx;
            }
            if (state.recentRect.Height() < state.minSize.cy) {
                state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
            }
        }
    } else {
        state.recentRect.right = state.recentRect.left + state.minSize.cx;
        state.recentRect.bottom = state.recentRect.top + state.minSize.cy;
    }

    m_rectBar = state.recentRect;
    state.visible = (m_hWnd != nullptr) ? TRUE : FALSE;
}
void CBasePane::RecalcLayout() {
    HWND hwnd = GetSafeHwnd();
    if (hwnd) {
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
    }
}

// ---------------------------------------------------------------------------
// Retail decode notes for the bodies added below (2026-09).
//
// Every RVA quoted here is a FUNCTION ENTRY in retail mfc140.dll (image base
// 0x180000000); where an individual instruction inside a body is meant, that is
// said explicitly.  mfc140.dll is the ANSI twin of the mfc140u.dll OpenMFC
// reimplements: the function BODIES are byte-identical, so control flow, member
// offsets and constants read off it are correct, but its RVAs are NOT mfc140u
// RVAs and none of the addresses below may be quoted as such.
//
// An indirect `call *0x1802c5bd0` in the retail listings is the Control Flow
// Guard dispatch (__guard_dispatch_icall_fptr), i.e. an ordinary indirect call
// through RAX -- not an import.  Every USER32/OLEAUT32 call named below was
// resolved through the import table, not guessed.
//
// The retail CBasePane vftable is at mfc140 RVA 0x2d9fa8; it was located from
// the CBasePane constructor (`lea 0x2cf258(%rip),%rax ; mov %rax,(%rdi)` at
// 0xad49, entry 0xad30).  Slot indices used below, read out of that table and
// cross-checked against the exported name at each slot:
//    31 (+0x0f8) 0x28a140 CWnd::OnToolHitTest
//    93 (+0x2e8) 0xb350   CBasePane::IsTabbed
//    94 (+0x2f0) 0x8780   IsFloating()  -- body is `!IsDocked()`
//   103 (+0x338) 0xce30   CBasePane::GetCurrentAlignment
//   104 (+0x340) 0x87d0   GetEnabledAlignment() -> m_dwEnabledAlignment (+0x100)
//   105 (+0x348) 0xc770   CBasePane::GetDockSiteFrameWnd
//   109 (+0x368) 0x87f0   GetRecentVisibleState() -> m_bRecentVisibleState (+0xec)
//   110 (+0x370) 0x8800   IsRestoredFromRegistry() -> +0xf0
//   114 (+0x390) 0x8840   GetPaneStyle() -> m_dwStyle (+0x104)
//   115 (+0x398) 0x8850   GetControlBarStyle() -> m_dwControlBarStyle (+0x108)
//   140 (+0x460) 0xb460   CBasePane::GetParentMiniFrame
//   147 (+0x498) 0x2820   CBasePane::OnUpdateCmdUI -- an empty `ret`
//   151 (+0x4b8) 0x3ae0   CBasePane::OnSetAccData -- `mov $1,%eax ; ret`
//   152 (+0x4c0) 0x3ae0   CBasePane::IsAccessibilityCompatible -- same folded body
//   155 (+0x4d8) 0xb230   CBasePane::DoPaint
// The slot numbering was verified end to end: slots 91..156 follow the
// declaration order of the new virtuals in the shipping afxbasepane.h, and
// every slot whose target is an exported name matches that order exactly.
//
// Other classes' vtables dispatched through below:
//   CMFCBaseTabCtrl vftable mfc140 RVA 0x2dbd88 (slot 0 = its GetThisClass at
//   0x156b0):  slot 137 (+0x448) 0x16ed0 GetTabFromHwnd,
//              slot 109 (+0x368) 0x15370 (unexported; the tab count),
//              slot 161 (+0x508) 0x16fc0 IsTabVisible.
//
// CRuntimeClass descriptors referenced below were identified by reading their
// m_lpszClassName: 0x2dba98 "CMFCBaseTabCtrl", 0x2db168 "CBaseTabbedPane",
// 0x3aa418 "CPaneFrameWnd", 0x3aa1a8 "CDockablePaneAdapter",
// 0x3aa5f8 "CMFCToolBar", 0x323910 "CDialog".
//
// Deliberate, shared deviations from retail, applied to every body below:
//  * a NULL check on `this` (and on out-pointer arguments where retail has
//    none).  Retail would fault; the rest of this file already guards that way
//    and a generated stub can hand these thunks anything.  Where retail DOES
//    already test an argument the test is reproduced exactly.
//  * where retail dispatches a virtual through the CBasePane vtable, OpenMFC's
//    CBasePane has no matching vtable, so the sibling impl__ thunk for the
//    CBasePane implementation is called instead.  That loses dispatch to a
//    derived override; each site says so.
// ---------------------------------------------------------------------------

// Thunks in other translation units.  BRIEFING S1: the C++ methods do not exist
// inside this DLL, only these extern "C" thunks.
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* pRect);
extern "C" void MS_ABI impl__MoveWindow_CWnd__QEAAXHHHHH_Z(
    CWnd* pThis, int x, int y, int nWidth, int nHeight, int bRepaint);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CWnd* pThis, CString* rString);
extern "C" OLECHAR* MS_ABI impl__AllocSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WXZ(
    void* pThis);
extern "C" int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(CWnd* pThis, void* lpResource);
extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI);
extern "C" __int64 MS_ABI impl__OnGetObject_CWnd__IEAA_J_K_J_Z(
    CWnd* pThis, unsigned __int64 wParam, __int64 lParam);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" CDockingManager* MS_ABI impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
    CGlobalUtils* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl__OnPaneContextMenu_CDockingManager__QEAAXVCPoint___Z(
    CDockingManager* pThis, CPoint pt);
extern "C" int MS_ABI impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(
    const CMFCBaseTabCtrl* pThis, HWND hWnd);
extern "C" int MS_ABI impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(
    const CMFCBaseTabCtrl* pThis, int iTab);
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);
// ?afxGlobalUtils@@3VCGlobalUtils@@A, defined at
// phase4/src/featurepack/CMFC_misc_stubs.cpp:3665.  GetDockingManager reads no
// member of it (core/runtime/CGlobalUtils.cpp:156), so passing its address is
// safe even though that storage is a 16-byte placeholder.
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[16];
// ?AFX_WM_TOOLBARMENU@@3IA, defined at phase4/src/core/runtime/Globals.cpp:191.
extern "C" unsigned int impl__AFX_WM_TOOLBARMENU__3IA;
// ?m_bRestoringDockState@CDockingManager@@2HA and
// ?m_dockModeGlobal@CDockingManager@@1W4AFX_DOCK_TYPE@@A, both defined at
// phase4/src/featurepack/docking/StaticData.cpp:50 and :56.
extern "C" int impl__m_bRestoringDockState_CDockingManager__2HA;
extern "C" unsigned int impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A;

// Sibling thunks defined further down in THIS file.
extern "C" int MS_ABI impl__OnSetAccData_CBasePane__UEAAHJ_Z(CBasePane* pThis, long lVal);
extern "C" int MS_ABI impl__IsTabbed_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" int MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis);
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert);
extern "C" void* MS_ABI impl__GetParentTabWnd_CBasePane__QEBAPEAVCMFCBaseTabCtrl__AEAPEAUHWND_____Z(
    const CBasePane* pThis, HWND* phWndTab);
extern "C" void MS_ABI impl__DoPaint_CBasePane__MEAAXPEAVCDC___Z(CBasePane* pThis, CDC* pDC);
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis);

namespace {
// Retail treats CHILDID_SELF as varChild.lVal == 0 with varChild.vt == VT_I4;
// the two helpers below just make the transcriptions below readable.
inline bool AccVtIsI4(const VARIANT* pv) { return pv != nullptr && pv->vt == VT_I4; }
inline long AccChildId(const VARIANT* pv) { return pv != nullptr ? pv->lVal : 0; }

// Retail reads a CString's length through the CStringData header that precedes
// the character buffer; this is the same test spelled with OpenMFC's CString.
inline bool AccStrEmpty(const CString& s) { return s.GetLength() == 0; }
inline BSTR AccStrToBSTR(const CString& s) {
    return impl__AllocSysString___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAPEA_WXZ(
        const_cast<CString*>(&s));
}
}  // namespace

// CBasePane::accHitTest -- transcribed from retail entry RVA 0xd620:
//     if (pvarChild == NULL) return E_INVALIDARG;
//     LONG pt = (WORD)xLeft | ((WORD)yTop << 16);
//     OnSetAccData(pt);                       // vtable slot 151
//     pvarChild->vt = VT_I4;
//     pvarChild->lVal = m_AccData.m_nAccHit != 0 ? pt : 0;   // m_nAccHit at +0x188
//     return S_OK;
// (`mov 0x188(%rsi),%eax ; neg %eax ; sbb %ecx,%ecx ; and %ebx,%ecx` is the
// conditional; note the packed point is passed to OnSetAccData, not a child id.)
// Deviation: the OnSetAccData call is made through this file's own thunk, so a
// derived override does not get it.
// Symbol: ?accHitTest@CBasePane@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CBasePane__UEAAJJJPEAUtagVARIANT___Z(
    CBasePane* pThis, long xLeft, long yTop, VARIANT* pvarChild) {
    if (pvarChild == nullptr) return E_INVALIDARG;
    if (pThis == nullptr) return E_INVALIDARG;
    long lPoint = static_cast<long>(
        static_cast<unsigned long>(static_cast<unsigned short>(xLeft)) |
        (static_cast<unsigned long>(static_cast<unsigned short>(yTop)) << 16));
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lPoint);
    pvarChild->vt = VT_I4;
    pvarChild->lVal = pThis->m_AccData.m_nAccHit != 0 ? lPoint : 0;
    return S_OK;
}

// CBasePane::accLocation -- transcribed from retail entry RVA 0xd520:
//     if (pxLeft == NULL || pyTop == NULL || pcxWidth == NULL || pcyHeight == NULL)
//         return E_INVALIDARG;
//     if (varChild.vt != VT_I4) return S_OK;
//     if (varChild.lVal == 0) {                       // CHILDID_SELF
//         CRect r(0,0,0,0); ::GetWindowRect(m_hWnd, &r);
//         *pxLeft = r.left; *pyTop = r.top;
//         *pcxWidth = r.right - r.left; *pcyHeight = r.bottom - r.top;
//         return S_OK;
//     }
//     if (varChild.lVal < 0) return S_OK;
//     OnSetAccData(varChild.lVal);                    // vtable slot 151
//     *pxLeft = m_AccData.m_rectAccLocation.left;     // +0x18c
//     *pyTop  = m_AccData.m_rectAccLocation.top;      // +0x190
//     *pcxWidth  = right - left;  *pcyHeight = bottom - top;
//     return S_OK;
// (0x1802c5370 resolves to USER32!GetWindowRect.)
// Symbol: ?accLocation@CBasePane@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CBasePane__UEAAJPEAJ000UtagVARIANT___Z(
    CBasePane* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight,
    const VARIANT* pvarChild) {
    if (pxLeft == nullptr || pyTop == nullptr || pcxWidth == nullptr ||
        pcyHeight == nullptr || pThis == nullptr) {
        return E_INVALIDARG;
    }
    if (!AccVtIsI4(pvarChild)) return S_OK;
    const long lChild = AccChildId(pvarChild);
    if (lChild == 0) {
        RECT rectWnd = {0, 0, 0, 0};
        ::GetWindowRect(pThis->m_hWnd, &rectWnd);
        *pxLeft = rectWnd.left;
        *pyTop = rectWnd.top;
        *pcxWidth = rectWnd.right - rectWnd.left;
        *pcyHeight = rectWnd.bottom - rectWnd.top;
        return S_OK;
    }
    if (lChild < 0) return S_OK;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    const CRect& rectAcc = pThis->m_AccData.m_rectAccLocation;
    *pxLeft = rectAcc.left;
    *pyTop = rectAcc.top;
    *pcxWidth = rectAcc.right - rectAcc.left;
    *pcyHeight = rectAcc.bottom - rectAcc.top;
    return S_OK;
}

// STUB.  Retail entry RVA 0xd4d0, fully decoded:
//     IAccessible* p = m_pStdObject;      // CWnd member at +0x50
//     if (p == NULL) return E_INVALIDARG;
//     return p->accSelect(flagsSelect, varChild);   // IAccessible vtable +0xa8
// OpenMFC's CWnd does not model m_pStdObject: its +0x50 is m_nFlags/m_dwStyle
// (include/openmfc/afxwin.h:589-593), so there is no accessible object to
// forward to and reading +0x50 as a pointer would read two unrelated DWORDs.
// See headerRequests.
// Symbol: ?accSelect@CBasePane@@UEAAJJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accSelect_CBasePane__UEAAJJUtagVARIANT___Z(
    CBasePane* pThis, long flagsSelect, const VARIANT* pvarChild) {
    (void)pThis;
    (void)flagsSelect;
    (void)pvarChild;
    return E_INVALIDARG;
}

// STUB.  Retail entry RVA 0xb790.  The body asks GetDockSiteFrameWnd() (vtable
// slot 105) for the owning frame and then, gated on the module flag at RVA
// 0x3aad00 being zero, dispatches on the frame's runtime class:
//   CFrameWndEx    -> CFrameWndEx::AddPane(pFrame, pBar, TRUE)      (0x67b30)
//   CMDIFrameWndEx -> CMDIFrameWndEx::AddPane(pFrame, pBar, TRUE)   (0x89690)
//   COleIPFrameWndEx / COleDocIPFrameWndEx -> CDockingManager::AddPane
//                     (0x476b0) on the manager embedded at frame+0x428
//   CMDIChildWndEx     -> same, manager at frame+0x3a8
//   COleCntrFrameWndEx -> same, manager at frame+0x1e0
// (Offsets and callees read straight off the listing; the flag test is
// `cmpl $0x0,0x39f540(%rip) ; jne` at 0xb7b9.)  The last four branches need the
// offset of the CDockingManager sub-object inside frame classes whose layouts
// OpenMFC does not model, and the flag itself is not modelled either, so the
// dispatch cannot be reproduced.  Implementing only the two CFrameWndEx/
// CMDIFrameWndEx branches would leave the other four frame types silently
// dropping the pane while looking implemented, so the whole body is left empty.
// Symbol: ?AddPane@CBasePane@@QEAAXPEAV1@@Z
extern "C" void MS_ABI impl__AddPane_CBasePane__QEAAXPEAV1__Z(CBasePane* pThis, CBasePane* pBar) {
    (void)pThis;
    (void)pBar;
}

// STUB.  Retail entry RVA 0xbdb0.  The body is
//     CPaneFrameWnd* p = GetParentMiniFrame(FALSE);   // vtable slot 140
//     if (p != NULL) { p->{slot 0x3a8}(hdwp); return; }
//     CWnd* pFrame = GetDockSiteFrameWnd();           // vtable slot 105
//     if (module flag at 0x3aad00 || pFrame == NULL) return;
//     then a six-way IsKindOf dispatch (CFrameWndEx, CMDIFrameWndEx,
//     COleIPFrameWndEx, COleDocIPFrameWndEx, CMDIChildWndEx,
//     COleCntrFrameWndEx) calling a different virtual slot on each frame class
//     (+0x390, +0x3a0, +0x3d8, +0x3d8, +0x3a8, +0x398) with hdwp.
// Every one of those slots is a virtual on a frame class whose vtable OpenMFC
// does not reproduce, so none of the calls can be made faithfully.
// Symbol: ?AdjustDockingLayout@CBasePane@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CBasePane__UEAAXPEAX_Z(CBasePane* pThis, void* hdwp) {
    (void)pThis;
    (void)hdwp;
}

// CBasePane::CopyState -- transcribed from retail entry RVA 0xce40.  The body
// is a straight-line copy of seven fields out of pOrgBar:
//     m_dwEnabledAlignment    = pOrgBar->GetEnabledAlignment();     // slot 104
//     m_bRecentVisibleState   = pOrgBar->GetRecentVisibleState();   // slot 109
//     m_bIsRestoredFromRegistry = pOrgBar->IsRestoredFromRegistry();// slot 110
//     m_pDockSite             = pOrgBar->GetDockSiteFrameWnd();     // slot 105
//     m_rectBar               = pOrgBar->m_rectBar;   // 16-byte movups/movdqu
//     m_bIsDlgControl         = pOrgBar->m_bIsDlgControl;           // +0xf4
//     m_dwStyle               = pOrgBar->GetPaneStyle();            // slot 114
//     m_dwControlBarStyle     = pOrgBar->GetControlBarStyle();      // slot 115
// Deviation: OpenMFC's CBasePane has no vtable slot for any of those five
// accessors.  For a plain CBasePane each of them returns exactly the member
// read directly here (slots 104/109/110/114/115 are the one-instruction bodies
// at 0x87d0/0x87f0/0x8800/0x8840/0x8850), and GetDockSiteFrameWnd is called
// through this file's own thunk.  A derived override of any of them is lost.
// Symbol: ?CopyState@CBasePane@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__CopyState_CBasePane__UEAAXPEAV1__Z(CBasePane* pThis, CBasePane* pOrgBar) {
    if (pThis == nullptr || pOrgBar == nullptr) return;
    pThis->m_dwEnabledAlignment = pOrgBar->m_dwEnabledAlignment;
    pThis->m_bRecentVisibleState = pOrgBar->m_bRecentVisibleState;
    pThis->m_bIsRestoredFromRegistry = pOrgBar->m_bIsRestoredFromRegistry;
    pThis->m_pDockSite = reinterpret_cast<CDockSite*>(
        impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(pOrgBar));
    pThis->m_rectBar = pOrgBar->m_rectBar;
    pThis->m_bIsDlgControl = pOrgBar->m_bIsDlgControl;
    pThis->m_dwStyle = pOrgBar->m_dwStyle;
    pThis->m_dwControlBarStyle = pOrgBar->m_dwControlBarStyle;
}

// STUB.  Retail entry RVA 0xbef0.  The body picks a frame --
//     CWnd* pFrame = bUseDockSite ? m_pDockSite       // +0x110
//                                 : AFXGetParentFrame(this);   // 0x6bb10
// -- and then, gated on the module flag at RVA 0x3aad00 being zero, runs the
// same six-way IsKindOf dispatch as AddPane above, calling
// CDockingManager::DockPane (0x483c0) on a manager embedded at frame+0x380,
// +0x228, +0x428 or +0x1e0, or CMDIChildWndEx::DockPane (0x85460).  The
// COleCntrFrameWndEx path additionally calls that frame's vtable slot +0x398.
// Same obstacle as AddPane: OpenMFC models none of those frame layouts.
// Symbol: ?DockPaneUsingRTTI@CBasePane@@QEAAXH@Z
extern "C" void MS_ABI impl__DockPaneUsingRTTI_CBasePane__QEAAXH_Z(CBasePane* pThis, int bUseDockSite) {
    (void)pThis;
    (void)bUseDockSite;
}

// STUB.  Retail entry RVA 0xb230:
//     CRect rectClip;   pDC->GetClipBox(rectClip);         // CDC vtable +0xb0
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);   // 0x1802c5358
//     CMFCVisualManager::GetInstance()->OnFillBarBackground(
//         pDC, this, rectClient, rectClip, FALSE);         // visual mgr vtable +0x78
// The two CRects go out in the shipping afxvisualmanager.h declaration order,
// rectClient BEFORE rectClip: in the outgoing frame at 0xb293-0xb2d3 the copy
// at [rsp+0x50] -- the ::GetClientRect result -- is what %r9 (argument 4) points
// at, and the copy at [rsp+0x40] -- the GetClipBox result -- is what the
// argument-5 stack slot [rsp+0x20] points at.  [rsp+0x28] is bNCArea = FALSE.
// Not implemented here for a link reason, not an information one: the base
// CMFCVisualManager::OnFillBarBackground does exist
// (phase4/src/featurepack/visualmanager/Thunks.cpp:1498 exports it, and
// CMFCVisualManager.cpp:618 is a real body), and so does the instance getter --
// but only as a real C++ static, CMFCVisualManager::GetInstance() at
// phase4/src/featurepack/visualmanager/CMFCVisualManager.cpp:593, called that
// way from e.g. phase4/src/featurepack/ribbon/CMFCRibbonButton.cpp:49.  There is
// no ?GetInstance@CMFCVisualManager@@SAPEAV1@XZ impl__ thunk, and BRIEFING S1
// forbids reaching a C++ method from this translation unit, so the forward
// cannot be written from here.  See headerRequests -- with that thunk this body
// is three lines.
// Symbol: ?DoPaint@CBasePane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CBasePane__MEAAXPEAVCDC___Z(CBasePane* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// STUB.  Retail entry RVA 0xd040:
//     if (*ppdispChild == NULL) return E_INVALIDARG;   // note: the retail test
//     if (varChild.vt == VT_I4 && varChild.lVal == 0) return E_INVALIDARG;
//     *ppdispChild = (IDispatch*)m_pStdObject;         // CWnd member at +0x50
//     return S_OK;
// (`cmpq $0x0,(%r8)` at 0xd040 really does dereference the out parameter rather
// than testing it for NULL.)  As with accSelect, OpenMFC's CWnd does not model
// m_pStdObject, so the only thing this could return is unrelated storage.
// Symbol: ?get_accChild@CBasePane@@UEAAJUtagVARIANT@@PEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accChild_CBasePane__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(
    CBasePane* pThis, const VARIANT* pvarChild, IDispatch** ppdispChild) {
    (void)pThis;
    (void)pvarChild;
    (void)ppdispChild;
    return E_INVALIDARG;
}

// CBasePane::get_accChildCount -- transcribed from retail entry RVA 0xcfd0,
// whose whole body is:
//     xor %eax,%eax ; test %rdx,%rdx ; jne + ; mov $0x80070057,%eax ; ret
//     + : mov %eax,(%rdx) ; ret
// i.e. `if (pcountChildren == NULL) return E_INVALIDARG; *pcountChildren = 0;
// return S_OK;`  (The export folded onto CMFCRibbonStatusBar's identical body,
// which is why the name does not appear in the RVA map; the address came out of
// CBasePane vftable slot 41.)
// Symbol: ?get_accChildCount@CBasePane@@UEAAJPEAJ@Z
extern "C" long MS_ABI impl__get_accChildCount_CBasePane__UEAAJPEAJ_Z(
    CBasePane* pThis, long* pcountChildren) {
    (void)pThis;
    if (pcountChildren == nullptr) return E_INVALIDARG;
    *pcountChildren = 0;
    return S_OK;
}

// CBasePane::get_accDefaultAction -- transcribed from retail entry RVA 0xd460:
//     if (varChild.vt != VT_I4 && varChild.lVal != 0) return E_INVALIDARG;
//     if (varChild.vt == VT_I4 && varChild.lVal == 0) return S_FALSE;
//     OnSetAccData(varChild.lVal);                     // vtable slot 151
//     if (m_AccData.m_strAccDefAction is empty) return S_FALSE;   // +0x178
//     *pszDefaultAction = m_strAccDefAction.AllocSysString();
//     return S_OK;
// (0xd4a9 sends a non-VT_I4 varChild with lVal == 0 into the SAME OnSetAccData
// path at 0xd47f, which is why the first test is a conjunction.)
// Symbol: ?get_accDefaultAction@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDefaultAction_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszDefaultAction) {
    if (pThis == nullptr || pszDefaultAction == nullptr) return E_INVALIDARG;
    const bool bI4 = AccVtIsI4(pvarChild);
    const long lChild = AccChildId(pvarChild);
    if (!bI4 && lChild != 0) return E_INVALIDARG;
    if (bI4 && lChild == 0) return S_FALSE;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strAccDefAction)) return S_FALSE;
    *pszDefaultAction = AccStrToBSTR(pThis->m_AccData.m_strAccDefAction);
    return S_OK;
}

// CBasePane::get_accDescription -- transcribed from retail entry RVA 0xd140:
//     if (varChild.vt != VT_I4) return S_FALSE;
//     if (varChild.lVal == 0) {                       // CHILDID_SELF
//         CString s; GetWindowText(s);                // 0x28a280
//         *pszDescription = s.AllocSysString(); return S_OK;
//     }
//     if (varChild.lVal < 0) return S_FALSE;
//     OnSetAccData(varChild.lVal);                    // vtable slot 151
//     if (m_AccData.m_strDescription is empty) return S_FALSE;   // +0x160
//     *pszDescription = m_strDescription.AllocSysString();
//     return S_OK;
// Symbol: ?get_accDescription@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accDescription_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszDescription) {
    if (pThis == nullptr || pszDescription == nullptr) return E_INVALIDARG;
    if (!AccVtIsI4(pvarChild)) return S_FALSE;
    const long lChild = AccChildId(pvarChild);
    if (lChild == 0) {
        CString strText;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            static_cast<const CWnd*>(pThis), &strText);
        *pszDescription = AccStrToBSTR(strText);
        return S_OK;
    }
    if (lChild < 0) return S_FALSE;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strDescription)) return S_FALSE;
    *pszDescription = AccStrToBSTR(pThis->m_AccData.m_strDescription);
    return S_OK;
}

// CBasePane::get_accFocus -- the export folded onto the body at RVA 0xd3b0
// (CBasePane vftable slot 51; the map keeps only CMFCPropertyGridCtrl's name for
// that address).  The whole body is
//     test %rdx,%rdx ; mov $0x80070057,%eax ; mov $0x80020003,%ecx
//     cmovne %ecx,%eax ; ret
// i.e. `return pvarChild == NULL ? E_INVALIDARG : DISP_E_MEMBERNOTFOUND;`
// Symbol: ?get_accFocus@CBasePane@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accFocus_CBasePane__UEAAJPEAUtagVARIANT___Z(
    CBasePane* pThis, VARIANT* pvarChild) {
    (void)pThis;
    return pvarChild == nullptr ? E_INVALIDARG : DISP_E_MEMBERNOTFOUND;
}

// CBasePane::get_accHelp -- transcribed from retail entry RVA 0xd330:
//     if (varChild.vt == VT_I4 && varChild.lVal == 0) {
//         *pszHelp = ::SysAllocString(L"ControlPane");   // literal at RVA 0x33aec0
//         return S_OK;
//     }
//     if (varChild.vt != VT_I4 && varChild.lVal != 0) return E_INVALIDARG;
//     if (pszHelp == NULL) return E_INVALIDARG;
//     OnSetAccData(varChild.lVal);                       // vtable slot 151
//     if (m_AccData.m_strAccHelp is empty) return S_FALSE;   // +0x170
//     *pszHelp = m_strAccHelp.AllocSysString();
//     return S_OK;
// (0x1802c49f8 is OLEAUT32 ordinal 2 == SysAllocString; the literal is a wide
// string even in the ANSI image.)
// Symbol: ?get_accHelp@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accHelp_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszHelp) {
    if (pThis == nullptr) return E_INVALIDARG;
    const bool bI4 = AccVtIsI4(pvarChild);
    const long lChild = AccChildId(pvarChild);
    if (bI4 && lChild == 0) {
        if (pszHelp == nullptr) return E_INVALIDARG;
        *pszHelp = ::SysAllocString(L"ControlPane");
        return S_OK;
    }
    if (!bI4 && lChild != 0) return E_INVALIDARG;
    if (pszHelp == nullptr) return E_INVALIDARG;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strAccHelp)) return S_FALSE;
    *pszHelp = AccStrToBSTR(pThis->m_AccData.m_strAccHelp);
    return S_OK;
}

// CBasePane::get_accHelpTopic -- the export folded onto the shared
// `mov $0x1,%eax ; ret` body at RVA 0x3ae0 (CBasePane vftable slot 49), i.e. it
// returns S_FALSE and touches none of its arguments.
// Symbol: ?get_accHelpTopic@CBasePane@@UEAAJPEAPEA_WUtagVARIANT@@PEAJ@Z
extern "C" long MS_ABI impl__get_accHelpTopic_CBasePane__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(
    CBasePane* pThis, BSTR* pszHelpFile, const VARIANT* pvarChild, long* pidTopic) {
    (void)pThis;
    (void)pszHelpFile;
    (void)pvarChild;
    (void)pidTopic;
    return S_FALSE;
}

// CBasePane::get_accKeyboardShortcut -- transcribed from retail entry RVA 0xd3d0:
//     if (varChild.vt == VT_I4 && varChild.lVal == 0) {
//         *pszKeyboardShortcut = ::SysAllocString(L"");  // empty literal, RVA 0x33aed8
//         return S_OK;
//     }
//     if (varChild.vt != VT_I4 && varChild.lVal != 0) return E_INVALIDARG;
//     if (pszKeyboardShortcut == NULL) return E_INVALIDARG;
//     if (varChild.vt != VT_I4) return S_FALSE;         // reached only with lVal == 0
//     if (varChild.lVal < 0) return S_FALSE;
//     OnSetAccData(varChild.lVal);                      // vtable slot 151
//     if (m_AccData.m_strAccKeys is empty) return S_FALSE;   // +0x168
//     *pszKeyboardShortcut = m_strAccKeys.AllocSysString();
//     return S_OK;
// Symbol: ?get_accKeyboardShortcut@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszKeyboardShortcut) {
    if (pThis == nullptr) return E_INVALIDARG;
    const bool bI4 = AccVtIsI4(pvarChild);
    const long lChild = AccChildId(pvarChild);
    if (bI4 && lChild == 0) {
        if (pszKeyboardShortcut == nullptr) return E_INVALIDARG;
        *pszKeyboardShortcut = ::SysAllocString(L"");
        return S_OK;
    }
    if (!bI4 && lChild != 0) return E_INVALIDARG;
    if (pszKeyboardShortcut == nullptr) return E_INVALIDARG;
    if (!bI4) return S_FALSE;
    if (lChild < 0) return S_FALSE;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strAccKeys)) return S_FALSE;
    *pszKeyboardShortcut = AccStrToBSTR(pThis->m_AccData.m_strAccKeys);
    return S_OK;
}

// CBasePane::get_accName -- transcribed from retail entry RVA 0xd070:
//     if (varChild.vt != VT_I4) return S_OK;      // note S_OK, not S_FALSE
//     if (varChild.lVal == 0) {                   // CHILDID_SELF
//         CString s; GetWindowText(s);            // 0x28a280
//         *pszName = s.AllocSysString(); return S_OK;
//     }
//     if (varChild.lVal < 0) return S_OK;
//     OnSetAccData(varChild.lVal);                // vtable slot 151
//     if (m_AccData.m_strAccName is empty) return S_FALSE;   // +0x150
//     *pszName = m_strAccName.AllocSysString();
//     return S_OK;
// (The two "return S_OK" paths jump to the shared `xor %eax,%eax` at 0xd125 and
// leave *pszName untouched -- this really does differ from get_accDescription,
// whose corresponding paths return S_FALSE.)
// Symbol: ?get_accName@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszName) {
    if (pThis == nullptr || pszName == nullptr) return E_INVALIDARG;
    if (!AccVtIsI4(pvarChild)) return S_OK;
    const long lChild = AccChildId(pvarChild);
    if (lChild == 0) {
        CString strText;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            static_cast<const CWnd*>(pThis), &strText);
        *pszName = AccStrToBSTR(strText);
        return S_OK;
    }
    if (lChild < 0) return S_OK;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strAccName)) return S_FALSE;
    *pszName = AccStrToBSTR(pThis->m_AccData.m_strAccName);
    return S_OK;
}

// STUB.  Retail entry RVA 0xcfe0, fully decoded:
//     if (ppdispParent == NULL) return E_INVALIDARG;
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (pParent == NULL) return E_INVALIDARG;
//     ::AccessibleObjectFromWindow(pParent->m_hWnd, OBJID_CLIENT,
//                                  IID_IAccessible, (void**)ppdispParent);
//     return *ppdispParent == NULL ? S_FALSE : S_OK;
// (`mov $0xfffffffc,%edx` is OBJID_CLIENT -- 0xFFFFFFFC; OBJID_WINDOW is 0 --
// the HRESULT the call returns is discarded; the GUID at RVA 0x2d7778 reads
// {618636E0-3D3C-11CF-810C-00AA00389B71} == IID_IAccessible; the call goes
// through the delay/AUX import slot at 0x1803e1148, i.e. OLEACC.)
// OpenMFC does not link oleacc -- phase4/scripts/build_phase4.sh:525-539 lists
// the import libraries and oleacc is not among them -- so calling
// AccessibleObjectFromWindow here would introduce an unresolvable import.
// See headerRequests.
// Symbol: ?get_accParent@CBasePane@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CBasePane__UEAAJPEAPEAUIDispatch___Z(
    CBasePane* pThis, IDispatch** ppdispParent) {
    (void)pThis;
    if (ppdispParent == nullptr) return E_INVALIDARG;
    return E_INVALIDARG;
}

// CBasePane::get_accRole -- transcribed from retail entry RVA 0xd210:
//     if (pvarRole == NULL) return E_INVALIDARG;
//     if (varChild.vt != VT_I4)  { pvarRole->lVal = 0x2b; pvarRole->vt = VT_I4; return S_OK; }
//     if (varChild.lVal == 0)    { pvarRole->lVal = 0x0a; pvarRole->vt = VT_I4; return S_OK; }
//     if (varChild.lVal < 0)     { pvarRole->lVal = 0x2b; pvarRole->vt = VT_I4; return S_OK; }
//     pvarRole->vt = VT_I4;
//     OnSetAccData(varChild.lVal);                 // vtable slot 151
//     pvarRole->lVal = m_AccData.m_nAccRole;       // +0x180
//     return S_OK;
// (0x0a is ROLE_SYSTEM_CLIENT and 0x2b is ROLE_SYSTEM_PUSHBUTTON in oleacc; the
// `mov %ax,(%r8)` that stores vt happens BEFORE the OnSetAccData call on the
// last path, which is why it is written first here too.)
// Symbol: ?get_accRole@CBasePane@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accRole_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(
    CBasePane* pThis, const VARIANT* pvarChild, VARIANT* pvarRole) {
    if (pvarRole == nullptr || pThis == nullptr) return E_INVALIDARG;
    const long lChild = AccChildId(pvarChild);
    if (!AccVtIsI4(pvarChild) || lChild < 0) {
        pvarRole->lVal = 0x2b;
        pvarRole->vt = VT_I4;
        return S_OK;
    }
    if (lChild == 0) {
        pvarRole->lVal = 0x0a;
        pvarRole->vt = VT_I4;
        return S_OK;
    }
    pvarRole->vt = VT_I4;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    pvarRole->lVal = static_cast<long>(pThis->m_AccData.m_nAccRole);
    return S_OK;
}

// CBasePane::get_accSelection -- the export folded onto the same body as
// get_accFocus at RVA 0xd3b0 (CBasePane vftable slot 52), so it is
//     return pvarChildren == NULL ? E_INVALIDARG : DISP_E_MEMBERNOTFOUND;
// Symbol: ?get_accSelection@CBasePane@@UEAAJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__get_accSelection_CBasePane__UEAAJPEAUtagVARIANT___Z(
    CBasePane* pThis, VARIANT* pvarChildren) {
    (void)pThis;
    return pvarChildren == nullptr ? E_INVALIDARG : DISP_E_MEMBERNOTFOUND;
}

// CBasePane::get_accState -- transcribed from retail entry RVA 0xd290:
//     if (pvarState == NULL) return E_INVALIDARG;
//     if (varChild.vt != VT_I4) return S_FALSE;
//     if (varChild.lVal == 0) {                       // CHILDID_SELF
//         pvarState->vt = VT_I4;
//         pvarState->lVal = IsKindOf(RUNTIME_CLASS(CMFCToolBar)) ? 0 : 0x100;
//         return S_OK;
//     }
//     if (varChild.lVal < 0) return S_FALSE;
//     OnSetAccData(varChild.lVal);                    // vtable slot 151
//     pvarState->vt = VT_I4;
//     pvarState->lVal = m_AccData.m_bAccState;        // +0x184
//     return S_OK;
// (The descriptor at RVA 0x3aa5f8 has m_lpszClassName "CMFCToolBar" and
// m_nObjectSize 4944.  `neg %rbx ; sbb %eax,%eax ; not %eax ; and $0x100,%eax`
// yields 0 when the IsKindOf succeeded and 0x100 -- STATE_SYSTEM_DEFAULT --
// when it did not.)
// Symbol: ?get_accState@CBasePane@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CBasePane__UEAAJUtagVARIANT__PEAU2__Z(
    CBasePane* pThis, const VARIANT* pvarChild, VARIANT* pvarState) {
    if (pvarState == nullptr || pThis == nullptr) return E_INVALIDARG;
    if (!AccVtIsI4(pvarChild)) return S_FALSE;
    const long lChild = AccChildId(pvarChild);
    if (lChild == 0) {
        pvarState->vt = VT_I4;
        pvarState->lVal = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                              static_cast<const CObject*>(pThis),
                              impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())
                              ? 0
                              : 0x100;
        return S_OK;
    }
    if (lChild < 0) return S_FALSE;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    pvarState->vt = VT_I4;
    pvarState->lVal = pThis->m_AccData.m_bAccState;
    return S_OK;
}

// CBasePane::get_accValue -- transcribed from retail entry RVA 0xd690:
//     if (varChild.vt != VT_I4) return S_FALSE;
//     if (varChild.lVal <= 0)   return S_FALSE;
//     OnSetAccData(varChild.lVal);                 // vtable slot 151
//     if (m_AccData.m_strAccValue is empty) return S_FALSE;   // +0x158
//     *pszValue = m_strAccValue.AllocSysString();
//     return S_OK;
// (There is no CHILDID_SELF special case here: `jle` at 0xd6ab takes lVal == 0
// down the S_FALSE path with the negatives.)
// Symbol: ?get_accValue@CBasePane@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accValue_CBasePane__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CBasePane* pThis, const VARIANT* pvarChild, BSTR* pszValue) {
    if (pThis == nullptr || pszValue == nullptr) return E_INVALIDARG;
    if (!AccVtIsI4(pvarChild)) return S_FALSE;
    const long lChild = AccChildId(pvarChild);
    if (lChild <= 0) return S_FALSE;
    impl__OnSetAccData_CBasePane__UEAAHJ_Z(pThis, lChild);
    if (AccStrEmpty(pThis->m_AccData.m_strAccValue)) return S_FALSE;
    *pszValue = AccStrToBSTR(pThis->m_AccData.m_strAccValue);
    return S_OK;
}

// CBasePane::GetCurrentAlignment -- retail entry RVA 0xce30 is three
// instructions: `mov 0x104(%rcx),%eax ; and $0xf000,%eax ; ret`, i.e.
// `return m_dwStyle & CBRS_ALIGN_ANY;`  (m_dwStyle is the CBasePane member at
// +0x104, not CWnd's cached style.)
// Symbol: ?GetCurrentAlignment@CBasePane@@UEBAKXZ
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane* pThis) {
    if (pThis == nullptr) return 0;
    return pThis->m_dwStyle & 0xF000UL;
}

// CBasePane::GetDockingMode -- retail entry RVA 0xc820:
//     mov 0x138(%rcx),%edx            ; m_dockMode
//     test %edx,%edx
//     mov 0x39e29a(%rip),%eax         ; CDockingManager::m_dockModeGlobal
//     cmovne %edx,%eax
// i.e. `return m_dockMode != 0 ? m_dockMode : CDockingManager::m_dockModeGlobal;`
// (The datum at RVA 0x3aaac8 is ?m_dockModeGlobal@CDockingManager@@1W4AFX_DOCK_TYPE@@A;
// OpenMFC defines it at phase4/src/featurepack/docking/StaticData.cpp:56.)
// Symbol: ?GetDockingMode@CBasePane@@UEBA?AW4AFX_DOCK_TYPE@@XZ
extern "C" int MS_ABI impl__GetDockingMode_CBasePane__UEBA_AW4AFX_DOCK_TYPE__XZ(const CBasePane* pThis) {
    if (pThis == nullptr) {
        return static_cast<int>(impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A);
    }
    return pThis->m_dockMode != 0
               ? pThis->m_dockMode
               : static_cast<int>(impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A);
}

// CBasePane::GetDockSiteFrameWnd -- retail entry RVA 0xc770.  The value the
// function returns is always `m_pDockSite` (the member at +0x110): the entry
// tests it and jumps straight to the shared `mov 0x110(%rbx),%rax` epilogue when
// it is non-NULL, and the only other path also falls into that epilogue.
// Everything between is a diagnostic that runs when m_pDockSite is NULL: if the
// parent window is a CDialog (descriptor at RVA 0x323910) and that parent's own
// HWND is the current thread's main window -- AfxGetModuleThreadState (0x1346a0),
// then the CWinThread at thread-state +0x8, then its vtable slot 31 (+0xf8,
// GetMainWnd), then that window's m_hWnd at +0x40 -- it sets the module-level
// DWORD at RVA 0x3aad00 to 1.  That is the same flag AddPane (0xb790, tested at
// 0xb7b9), InsertPane, AdjustDockingLayout and DockPaneUsingRTTI read to
// suppress docking for panes hosted in a dialog.
// Deviation: OpenMFC models no such global, so only the return value is
// reproduced and the flag is never set.  Nothing in this DLL reads it.
// Symbol: ?GetDockSiteFrameWnd@CBasePane@@UEBAPEAVCWnd@@XZ
extern "C" void* MS_ABI impl__GetDockSiteFrameWnd_CBasePane__UEBAPEAVCWnd__XZ(const CBasePane* pThis) {
    if (pThis == nullptr) return nullptr;
    return pThis->m_pDockSite;
}

// CBasePane::GetParentMiniFrame -- transcribed from retail entry RVA 0xb460:
//     HWND h = m_hWnd;
//     for (;;) {
//         CWnd* p = CWnd::FromHandle(::GetParent(h));
//         if (p == NULL) return NULL;
//         if (p->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) return p;
//         h = p->m_hWnd;
//     }
// The bNoAssert argument is not read anywhere in the body.  (Retail re-tests
// IsKindOf once more at 0xb4a1 before returning; that second test can only
// succeed, so it is not reproduced.)
// Symbol: ?GetParentMiniFrame@CBasePane@@UEBAPEAVCPaneFrameWnd@@H@Z
extern "C" void* MS_ABI impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(
    const CBasePane* pThis, int bNoAssert) {
    (void)bNoAssert;
    if (pThis == nullptr) return nullptr;
    HWND hWnd = pThis->m_hWnd;
    for (;;) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd));
        if (pParent == nullptr) return nullptr;
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                static_cast<const CObject*>(pParent),
                impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            return pParent;
        }
        hWnd = pParent->m_hWnd;
    }
}

// CBasePane::GetParentTabbedPane -- transcribed from retail entry RVA 0xc9b0:
//     if (!IsTabbed()) return NULL;                   // vtable slot 93
//     HWND hWndTab = NULL;
//     CMFCBaseTabCtrl* pTab = GetParentTabWnd(hWndTab);
//     if (hWndTab == NULL || pTab == NULL) return NULL;
//     CWnd* p = CWnd::FromHandle(::GetParent(pTab->m_hWnd));
//     if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return NULL;
//     return (CBaseTabbedPane*)p;
// (The descriptor at RVA 0x2db168 reads "CBaseTabbedPane", m_nObjectSize 1304.)
// Deviation: IsTabbed is called through this file's own thunk, so a derived
// override is not seen.
// Symbol: ?GetParentTabbedPane@CBasePane@@QEBAPEAVCBaseTabbedPane@@XZ
extern "C" void* MS_ABI impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(
    const CBasePane* pThis) {
    if (pThis == nullptr) return nullptr;
    if (!impl__IsTabbed_CBasePane__UEBAHXZ(pThis)) return nullptr;
    HWND hWndTab = nullptr;
    CWnd* pTab = static_cast<CWnd*>(
        impl__GetParentTabWnd_CBasePane__QEBAPEAVCMFCBaseTabCtrl__AEAPEAUHWND_____Z(pThis, &hWndTab));
    if (hWndTab == nullptr || pTab == nullptr) return nullptr;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pTab->m_hWnd));
    if (pParent == nullptr) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParent),
            impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pParent;
}

// CBasePane::GetParentTabWnd -- transcribed from retail entry RVA 0xc8d0:
//     CWnd* pBase = this;
//     CWnd* p = CWnd::FromHandle(::GetParent(m_hWnd));
//     if (p != NULL && p->IsKindOf(RUNTIME_CLASS(CDockablePaneAdapter)))
//         pBase = p;
//     hWndTab = pBase->m_hWnd;                      // written unconditionally
//     CWnd* q = CWnd::FromHandle(::GetParent(pBase->m_hWnd));
//     if (q != NULL && q->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl)))
//         return (CMFCBaseTabCtrl*)q;
//     CWnd* r = CWnd::FromHandle(::GetParent(pBase->m_hWnd));
//     if (r == NULL || !r->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return NULL;
//     return r->{CBaseTabbedPane vtable slot 234}();
// (Descriptors: 0x3aa1a8 "CDockablePaneAdapter", 0x2dba98 "CMFCBaseTabCtrl",
// 0x2db168 "CBaseTabbedPane".)
// Deviation: the final call is CBaseTabbedPane vtable slot 234 (+0x750), an
// unexported accessor that hands back the tab control inside a tabbed pane.
// OpenMFC reproduces neither that vtable nor an equivalent export, so that last
// path returns NULL here.  Everything before it is faithful.
// Symbol: ?GetParentTabWnd@CBasePane@@QEBAPEAVCMFCBaseTabCtrl@@AEAPEAUHWND__@@@Z
extern "C" void* MS_ABI impl__GetParentTabWnd_CBasePane__QEBAPEAVCMFCBaseTabCtrl__AEAPEAUHWND_____Z(
    const CBasePane* pThis, HWND* phWndTab) {
    if (pThis == nullptr || phWndTab == nullptr) return nullptr;
    const CWnd* pBase = static_cast<const CWnd*>(pThis);
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pParent),
            impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ())) {
        pBase = pParent;
    }
    *phWndTab = pBase->m_hWnd;
    CWnd* pTabCandidate = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pBase->m_hWnd));
    if (pTabCandidate != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            static_cast<const CObject*>(pTabCandidate),
            impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ())) {
        return pTabCandidate;
    }
    return nullptr;
}

// CBasePane::HandleInitDialog -- transcribed from retail entry RVA 0xcca0.
// Both message parameters are ignored; the body is
//     if (m_lpszBarTemplateName != NULL &&           // +0x140
//         !ExecuteDlgInit(m_lpszBarTemplateName))    // 0x28f850
//         return 0;
//     return UpdateData(FALSE) != 0;                 // 0x28f530
// Symbol: ?HandleInitDialog@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__HandleInitDialog_CBasePane__IEAA_J_K_J_Z(
    CBasePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    if (pThis == nullptr) return 0;
    if (pThis->m_lpszBarTemplateName != nullptr &&
        !impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(
            static_cast<CWnd*>(pThis),
            const_cast<wchar_t*>(pThis->m_lpszBarTemplateName))) {
        return 0;
    }
    return impl__UpdateData_CWnd__QEAAHH_Z(static_cast<CWnd*>(pThis), FALSE) != 0 ? 1 : 0;
}

// STUB.  Retail entry RVA 0xbc50.  The body first tries the mini frame --
//     CPaneFrameWnd* p = GetParentMiniFrame(FALSE);          // vtable slot 140
//     if (p != NULL && p->IsKindOf(RUNTIME_CLASS(CMultiPaneFrameWnd)))  // 0x3aa2f8
//         return p->{slot 0x4c0}(pControlBar, pTarget, bAfter);
// -- and otherwise runs the same frame dispatch as AddPane over
// GetDockSiteFrameWnd() (vtable slot 105), calling CDockingManager::InsertPane
// (0x477a0) on the manager embedded at frame+0x380 (CFrameWndEx), +0x228
// (CMDIFrameWndEx), +0x428 (COleIPFrameWndEx and COleDocIPFrameWndEx), +0x3a8
// (CMDIChildWndEx) or +0x1e0 (COleCntrFrameWndEx).  It returns TRUE when there
// is no dock-site frame or the module flag at 0x3aad00 is set, and FALSE when
// the frame matched none of the six runtime classes.  As with AddPane those
// frame-internal docking-manager offsets are not modelled here.
// Symbol: ?InsertPane@CBasePane@@QEAAHPEAV1@0H@Z
extern "C" int MS_ABI impl__InsertPane_CBasePane__QEAAHPEAV1_0H_Z(
    CBasePane* pThis, CBasePane* pControlBar, CBasePane* pTarget, int bAfter) {
    (void)pThis;
    (void)pControlBar;
    (void)pTarget;
    (void)bAfter;
    return FALSE;
}

// CBasePane::IsDocked -- the export folded onto the body at RVA 0xb300 that the
// map names ?IsDocked@CDockablePane@@UEBAHXZ (CBasePane vftable slot 92):
//     CPaneFrameWnd* p = GetParentMiniFrame(FALSE);   // vtable slot 140
//     if (p == NULL) return TRUE;
//     return p->{CPaneFrameWnd vtable slot 106}() != 1;
// (`xor %ecx,%ecx ; cmp $0x1,%eax ; setne %cl` -- it is != 1, not > 1.)
// Deviation: slot 106 of CPaneFrameWnd is the pane count, which this DLL does
// not export and whose vtable it does not reproduce, so the "inside a mini
// frame" branch returns FALSE -- what retail returns for the ordinary
// single-pane mini frame.  This matches the identical decision already taken in
// CDockablePane::IsDocked (phase4/src/featurepack/docking/CDockablePane.cpp:868).
// Symbol: ?IsDocked@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsDocked_CBasePane__UEBAHXZ(const CBasePane* pThis) {
    if (pThis == nullptr) return FALSE;
    if (impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pThis, FALSE) == nullptr) {
        return TRUE;
    }
    return FALSE;
}

// STUB.  Retail entry RVA 0xb9d0 is the same frame dispatch as AddPane and
// InsertPane:  it takes GetDockSiteFrameWnd() (vtable slot 105), returns TRUE
// when that is NULL or the module flag at 0x3aad00 is set, and otherwise
// forwards to CDockingManager::IsPointNearDockSite (0x48160) on the manager
// embedded at frame+0x380, +0x228, +0x428, +0x3a8 or +0x1e0 depending on the
// frame's runtime class, returning FALSE if none of the six matched.  Those
// frame-internal offsets are not modelled here, so the forwarding cannot be
// reproduced.
// Symbol: ?IsPointNearDockSite@CBasePane@@QEBAHVCPoint@@AEAKAEAH@Z
extern "C" int MS_ABI impl__IsPointNearDockSite_CBasePane__QEBAHVCPoint__AEAKAEAH_Z(
    const CBasePane* pThis, long long point, unsigned long* dwBarAlignment, int* bOuterEdge) {
    (void)pThis;
    (void)point;
    (void)dwBarAlignment;
    (void)bOuterEdge;
    return FALSE;
}

// CBasePane::IsMDITabbed -- retail entry RVA 0xb380 is two instructions:
// `mov 0xf8(%rcx),%eax ; ret`, i.e. `return m_bIsMDITabbed;` (+0xf8).
// Symbol: ?IsMDITabbed@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsMDITabbed_CBasePane__UEBAHXZ(const CBasePane* pThis) {
    if (pThis == nullptr) return FALSE;
    return pThis->m_bIsMDITabbed;
}

// CBasePane::IsTabbed -- transcribed from retail entry RVA 0xb350, whose whole
// body is a tail jump:
//     return CWnd::FromHandle(::GetParent(m_hWnd))
//                ->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl));
// (0x1802c5300 is USER32!GetParent, 0x289180 is CWnd::FromHandle, 0x233310 is
// CObject::IsKindOf, and the descriptor at 0x2dba98 reads "CMFCBaseTabCtrl".)
// Note retail passes the FromHandle result straight to IsKindOf; IsKindOf itself
// tests it for NULL first (0x233319), so a NULL parent yields FALSE.
// Symbol: ?IsTabbed@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsTabbed_CBasePane__UEBAHXZ(const CBasePane* pThis) {
    if (pThis == nullptr) return FALSE;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        static_cast<const CObject*>(pParent),
        impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ());
}

// CBasePane::IsVisible -- transcribed from retail entry RVA 0xb390:
//     if (IsTabbed()) {                                  // vtable slot 93
//         HWND hWndTab = NULL;
//         CMFCBaseTabCtrl* pTab = GetParentTabWnd(hWndTab);      // 0xc8d0
//         if (!::IsWindowVisible(pTab->m_hWnd)) return FALSE;
//         int i = pTab->GetTabFromHwnd(hWndTab);         // tab vtable slot 137
//         if (i < 0 || i >= pTab->{tab slot 109}()) return FALSE;   // tab count
//         return pTab->IsTabVisible(i);                  // tab vtable slot 161
//     }
//     if (CDockingManager::m_bRestoringDockState)        // DWORD at RVA 0x3b6f50
//         return GetRecentVisibleState();                // vtable slot 109
//     return (GetStyle() & WS_VISIBLE) != 0;             // 0x2a75a0, bit 28
// Deviations: IsTabbed and GetParentTabWnd are called through this file's own
// thunks (a derived override is lost), GetRecentVisibleState is read directly as
// m_bRecentVisibleState (+0xec -- that is exactly what slot 109's one-instruction
// body at 0x87f0 does for CBasePane), and the tab-count bound is dropped because
// the tab count is an unexported inline; OpenMFC's
// impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z range-checks the index itself.
// Symbol: ?IsVisible@CBasePane@@UEBAHXZ
extern "C" int MS_ABI impl__IsVisible_CBasePane__UEBAHXZ(const CBasePane* pThis) {
    if (pThis == nullptr) return FALSE;
    if (impl__IsTabbed_CBasePane__UEBAHXZ(pThis)) {
        HWND hWndTab = nullptr;
        CWnd* pTab = static_cast<CWnd*>(
            impl__GetParentTabWnd_CBasePane__QEBAPEAVCMFCBaseTabCtrl__AEAPEAUHWND_____Z(pThis, &hWndTab));
        if (pTab == nullptr) return FALSE;
        if (!::IsWindowVisible(pTab->m_hWnd)) return FALSE;
        int iTab = impl__GetTabFromHwnd_CMFCBaseTabCtrl__UEBAHPEAUHWND_____Z(
            reinterpret_cast<const CMFCBaseTabCtrl*>(pTab), hWndTab);
        if (iTab < 0) return FALSE;
        return impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(
            reinterpret_cast<const CMFCBaseTabCtrl*>(pTab), iTab);
    }
    if (impl__m_bRestoringDockState_CDockingManager__2HA) {
        return pThis->m_bRecentVisibleState;
    }
    return (impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & WS_VISIBLE) != 0;
}

// STUB.  Retail entry RVA 0xc490 (the Unicode export has the same body).  It
// builds a registry path with AFXGetRegPath("BasePanes", lpszProfileName)
// (0xd2540), substitutes nIndex with GetDlgCtrlID() when it is -1, formats
// "%TsBasePane-%d" (literals at RVA 0x33ae78 and 0x33ae88) and then reads the
// pane's persisted state from that key.  OpenMFC exports no AFXGetRegPath and
// models none of the profile plumbing the rest of the body drives; guessing a
// registry layout here would read state that SaveState -- still a stub in this
// file -- never writes.
// Symbol: ?LoadState@CBasePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CBasePane__UEAAHPEB_WHI_Z(
    CBasePane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis;
    (void)lpszProfileName;
    (void)nIndex;
    (void)uiID;
    return FALSE;
}

// CBasePane::MoveWindow -- transcribed from retail entry RVA 0xb580:
//     CRect rectWnd(0,0,0,0);
//     ::GetWindowRect(m_hWnd, &rectWnd);
//     CWnd* pParent = IsFloating()                      // vtable slot 94
//                       ? (CWnd*)GetParentMiniFrame(FALSE)   // vtable slot 140
//                       : m_pDockSite;                       // +0x110
//     if (pParent != NULL) pParent->ScreenToClient(&rectWnd);
//     if (::EqualRect(&rectWnd, &rect)) return hdwp;
// -- with one caveat: retail tests for NULL only on the m_pDockSite branch (at
// 0xb5f6).  The mini-frame result is handed to ScreenToClient unchecked, so
// retail would fault there; the guard below covers both branches instead.
//     if (hdwp != NULL)
//         return ::DeferWindowPos(hdwp, m_hWnd, NULL, rect.left, rect.top,
//                                 rect.Width(), rect.Height(),
//                                 SWP_NOZORDER | SWP_NOACTIVATE);
//     CWnd::MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), bRepaint);
//     return NULL;
// (0x1802c5370/0x1802c52e8/0x1802c52e0 resolve to USER32 GetWindowRect,
// EqualRect and DeferWindowPos; the literal 0x14 in the DeferWindowPos frame is
// SWP_NOZORDER|SWP_NOACTIVATE.  Slot 94's body at 0x8780 is `!IsDocked()`, so
// IsFloating is spelled that way below.)
// Deviation: IsDocked and GetParentMiniFrame go through this file's own thunks,
// losing dispatch to a derived override.
// Symbol: ?MoveWindow@CBasePane@@UEAAPEAXAEBVCRect@@HPEAX@Z
extern "C" void* MS_ABI impl__MoveWindow_CBasePane__UEAAPEAXAEBVCRect__HPEAX_Z(
    CBasePane* pThis, const RECT* lpRect, int bRepaint, void* hdwp) {
    if (pThis == nullptr || lpRect == nullptr) return hdwp;
    RECT rectWnd = {0, 0, 0, 0};
    ::GetWindowRect(pThis->m_hWnd, &rectWnd);
    CWnd* pParent = nullptr;
    if (!impl__IsDocked_CBasePane__UEBAHXZ(pThis)) {
        pParent = static_cast<CWnd*>(
            impl__GetParentMiniFrame_CBasePane__UEBAPEAVCPaneFrameWnd__H_Z(pThis, FALSE));
    } else {
        pParent = reinterpret_cast<CWnd*>(pThis->m_pDockSite);
    }
    if (pParent != nullptr) {
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(pParent, &rectWnd);
    }
    if (::EqualRect(&rectWnd, lpRect)) return hdwp;
    const int cx = lpRect->right - lpRect->left;
    const int cy = lpRect->bottom - lpRect->top;
    if (hdwp != nullptr) {
        return ::DeferWindowPos(static_cast<HDWP>(hdwp), pThis->m_hWnd, nullptr,
                                lpRect->left, lpRect->top, cx, cy,
                                SWP_NOZORDER | SWP_NOACTIVATE);
    }
    impl__MoveWindow_CWnd__QEAAXHHHHH_Z(static_cast<CWnd*>(pThis), lpRect->left,
                                        lpRect->top, cx, cy, bRepaint);
    return nullptr;
}

// CBasePane::OnEraseBkgnd -- the export has no RVA of its own in the map and is
// not a virtual, so it is not in the vftable; it IS reachable through the class
// message map.  CBasePane::GetThisMessageMap (RVA 0xaf60) returns the AFX_MSGMAP
// at RVA 0x2d9e28, whose entry array at 0x2d9e40 contains
//     { nMessage = 0x0014 (WM_ERASEBKGND), nCode = 0, nID = 0, nLastID = 0,
//       nSig = 0x1, pfn = RVA 0x3ae0 }
// and 0x3ae0 is the shared `mov $0x1,%eax ; ret` body.  The handler is therefore
// `return TRUE;` -- the pane paints its own background in DoPaint, so the
// default class-brush erase is suppressed.  (The neighbouring entries in that
// same array resolve to the named CBasePane handlers -- 0xb4d0 OnPaint,
// 0xc320 OnSize, 0xc270 OnIdleUpdateCmdUI, 0xca30 OnHelpHitTest,
// 0xcca0 HandleInitDialog, 0xcdb0 OnGetObject, 0xd6f0 OnPrintClient -- which is
// what identifies the array.)
// Note the consequence while DoPaint above is still a documented stub: TRUE
// suppresses the default class-brush erase and nothing repaints the background,
// so a plain CBasePane leaves its client area undrawn.  That is what retail
// does; the fix belongs in DoPaint, not here.
// Symbol: ?OnEraseBkgnd@CBasePane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CBasePane__IEAAHPEAVCDC___Z(CBasePane* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}

// CBasePane::OnGetObject -- transcribed from retail entry RVA 0xcdb0:
//     if (afxGlobalData not yet initialised) AFX_GLOBAL_DATA::Initialize();
//     if (<accessibility-support flag inside afxGlobalData, at RVA 0x3ba5f8> &&
//         IsAccessibilityCompatible())               // vtable slot 152
//         return CWnd::OnGetObject(wParam, lParam);  // 0x28da30
//     return 0;
// (0x3ba380 is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A and 0x6a5c0 is
// AFX_GLOBAL_DATA::Initialize; the tested DWORD at 0x3ba5f8 sits inside that
// structure, 0x278 bytes in.)
// Deviations: OpenMFC has no AFX_GLOBAL_DATA lazy-initialisation entry point and
// does not model that flag, so both are treated as "already initialised" and
// "accessibility available".  IsAccessibilityCompatible IS dispatched virtually
// here -- OpenMFC declares it on CBasePane (include/openmfc/afxmfc.h:375) and it
// is defined in this file -- so a derived override is honoured.
// Symbol: ?OnGetObject@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnGetObject_CBasePane__IEAA_J_K_J_Z(
    CBasePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    if (!pThis->IsAccessibilityCompatible()) return 0;
    return impl__OnGetObject_CWnd__IEAA_J_K_J_Z(static_cast<CWnd*>(pThis), wParam, lParam);
}

// CBasePane::OnHelpHitTest -- transcribed from retail entry RVA 0xca30.  wParam
// is ignored; the body is
//     CPoint pt((short)LOWORD(lParam), (short)HIWORD(lParam));
//     LRESULT hit = OnToolHitTest(pt, NULL);       // vtable slot 31 == CWnd's
//     if (hit != -1) return hit + 0x10000;         // HID_BASE_COMMAND
//     UINT id = ::GetDlgCtrlID(m_hWnd);
//     return id != 0 ? id + 0x50000 : 0;           // HID_BASE_CONTROL
// (`neg %rax ; sbb %rax,%rax ; and %rcx,%rax` at 0xca86 is the final ternary;
// 0x1802c52d0 resolves to USER32!GetDlgCtrlID.)
// Symbol: ?OnHelpHitTest@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnHelpHitTest_CBasePane__IEAA_J_K_J_Z(
    CBasePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    if (pThis == nullptr) return 0;
    CPoint point(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)));
    LONGLONG hit = impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
        static_cast<const CWnd*>(pThis), point, nullptr);
    if (hit != -1) return static_cast<__int64>(hit) + 0x10000;
    unsigned long nID = static_cast<unsigned long>(
        impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pThis)));
    return nID != 0 ? static_cast<__int64>(nID) + 0x50000 : 0;
}

// CBasePane::OnIdleUpdateCmdUI -- transcribed from retail entry RVA 0xc270:
//     if (!(GetStyle() & WS_VISIBLE)) return 0;                  // bit 28
//     if (m_pParentDockBar != NULL &&                            // +0x128
//         !(m_pParentDockBar->GetStyle() & WS_VISIBLE)) return 0;
//     CWnd* pOwner = CWnd::FromHandle(m_hWndOwner != NULL ? m_hWndOwner
//                                                         : ::GetParent(m_hWnd));
//     CWnd* pTarget = (pOwner != NULL && pOwner->{CWnd vtable slot 86}())
//                       ? pOwner : AFXGetParentFrame(this);      // 0x6bb10
//     if (pTarget == NULL) return 0;
//     OnUpdateCmdUI((CFrameWnd*)pTarget, (BOOL)wParam);          // vtable slot 147
//     return 0;
// Slot 86 is a CWnd virtual whose CWnd body is the shared `xor %eax,%eax ; ret`
// at 0x7260; the "owner is itself a frame" role matches CWnd::IsFrameWnd.  That
// identification is by role and by the FALSE-returning CWnd body, not by a name
// read out of the map, and nothing below calls it -- see the deviation note.
// (+0xa0 is CWnd::m_hWndOwner: CControlBar::SetOwner, RVA 0x1d93a0, stores the
// owner HWND to that offset at 0x1d93ef.)
// Deviation: only the two visibility guards are reproduced.  OpenMFC's CBasePane
// has no vtable slot 147, and slot 147 for a plain CBasePane is the empty `ret`
// at 0x2820, so the target lookup would have nothing to hand the result to; both
// it and the OnUpdateCmdUI call are therefore omitted, which loses the update
// pass for a derived pane that overrides OnUpdateCmdUI.  (For the record the
// lookup itself is: CWnd::FromHandle(m_hWndOwner ? m_hWndOwner
// : ::GetParent(m_hWnd)), then that window if its slot 86 -- the CWnd virtual
// whose CWnd body is the FALSE-returning 0x7260, i.e. the "is this a frame"
// test -- says yes, else AFXGetParentFrame(this).)
// Symbol: ?OnIdleUpdateCmdUI@CBasePane@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CBasePane__IEAA_J_K_J_Z(
    CBasePane* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    if (pThis == nullptr) return 0;
    if ((impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & WS_VISIBLE) == 0) {
        return 0;
    }
    CWnd* pParentDockBar = reinterpret_cast<CWnd*>(pThis->m_pParentDockBar);
    if (pParentDockBar != nullptr &&
        (impl__GetStyle_CWnd__QEBAKXZ(pParentDockBar) & WS_VISIBLE) == 0) {
        return 0;
    }
    return 0;
}

// CBasePane::OnPaint -- transcribed from retail entry RVA 0xb4d0:
//     if (CBasePane::m_bMultiThreaded)                 // DWORD at RVA 0x3b6f00
//         ::EnterCriticalSection(&CBasePane::m_CriticalSection);   // 0x3aabd0
//     CPaintDC dc(this);                               // 0x2a1c60
//     if (GetStyle() & WS_VISIBLE) DoPaint(&dc);       // vtable slot 155
//     if (CBasePane::m_bMultiThreaded)
//         ::LeaveCriticalSection(&CBasePane::m_CriticalSection);
//     // ~CPaintDC runs last, AFTER the LeaveCriticalSection (0x2a1d10)
// Deviations: OpenMFC does not model CBasePane::m_CriticalSection (the static
// CCriticalSection declared in the shipping afxbasepane.h), so the two guarded
// critical-section calls are omitted; m_bMultiThreaded defaults to FALSE, which
// is the path where retail omits them too.  DoPaint is called through this
// file's own thunk (currently a documented stub), losing derived dispatch.
// Symbol: ?OnPaint@CBasePane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CBasePane__IEAAXXZ(CBasePane* pThis) {
    if (pThis == nullptr) return;
    alignas(void*) unsigned char dcStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(dcStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, static_cast<CWnd*>(pThis));
    if ((impl__GetStyle_CWnd__QEBAKXZ(static_cast<const CWnd*>(pThis)) & WS_VISIBLE) != 0) {
        impl__DoPaint_CBasePane__MEAAXPEAVCDC___Z(pThis, static_cast<CDC*>(pDC));
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// CBasePane::OnPaneContextMenu -- transcribed from retail entry RVA 0xcf50:
//     LRESULT lr = ::SendMessage(pParentFrame->m_hWnd, AFX_WM_TOOLBARMENU,
//                                (WPARAM)(this != NULL ? m_hWnd : NULL),
//                                MAKELPARAM(point.x, point.y));
//     if (lr == 0) return;                              // note: work happens when != 0
//     CFrameWnd* pFrame = GetParentFrame();             // 0x28c680
//     CDockingManager* p = afxGlobalUtils.GetDockingManager(pFrame);   // 0x6ccc0
//     if (p != NULL) p->OnPaneContextMenu(point);       // 0x4e720
// (0x1802c5378 is USER32!SendMessageA in this ANSI image, i.e. SendMessageW in
// mfc140u -- ::SendMessage below picks the right one under UNICODE.  The message
// id at RVA 0x3bb21c is ?AFX_WM_TOOLBARMENU@@3IA.)
// Symbol: ?OnPaneContextMenu@CBasePane@@UEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CBasePane__UEAAXPEAVCWnd__VCPoint___Z(
    CBasePane* pThis, CWnd* pParentFrame, long long point) {
    if (pParentFrame == nullptr) return;
    const int ptx = static_cast<int>(static_cast<long>(point & 0xFFFFFFFFll));
    const int pty = static_cast<int>(static_cast<long>((point >> 32) & 0xFFFFFFFFll));
    LRESULT lr = ::SendMessage(
        pParentFrame->m_hWnd, impl__AFX_WM_TOOLBARMENU__3IA,
        reinterpret_cast<WPARAM>(pThis != nullptr ? pThis->m_hWnd : nullptr),
        static_cast<LPARAM>(MAKELONG(static_cast<unsigned short>(ptx),
                                     static_cast<unsigned short>(pty))));
    if (lr == 0) return;
    if (pThis == nullptr) return;
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
        static_cast<const CWnd*>(pThis));
    CDockingManager* pManager = impl__GetDockingManager_CGlobalUtils__QEAAPEAVCDockingManager__PEAVCWnd___Z(
        reinterpret_cast<CGlobalUtils*>(impl__afxGlobalUtils__3VCGlobalUtils__A),
        static_cast<CWnd*>(pFrame));
    if (pManager == nullptr) return;
    impl__OnPaneContextMenu_CDockingManager__QEAAXVCPoint___Z(pManager, CPoint(ptx, pty));
}

// CBasePane::OnPrintClient -- transcribed from retail entry RVA 0xd6f0:
//     if (nFlags & PRF_ERASEBKGND)                        // 0x8
//         ::SendMessage(m_hWnd, WM_ERASEBKGND,
//                       (WPARAM)(pDC != NULL ? pDC->m_hDC : NULL), 0);
//     if (nFlags & PRF_CLIENT)                            // 0x4
//         DoPaint(pDC);                                   // vtable slot 155
//     return 0;
// (`lea 0x14(%r9),%edx` with %r9 == 0 materialises WM_ERASEBKGND; the wParam is
// read from pDC+0x8, the CDC::m_hDC member.  0x1802c5378 is USER32!SendMessageA
// in this ANSI image, so ::SendMessage is correct for mfc140u.)
// Deviation: DoPaint goes through this file's own thunk, losing derived dispatch.
// Symbol: ?OnPrintClient@CBasePane@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CBasePane__IEAA_JPEAVCDC__I_Z(
    CBasePane* pThis, CDC* pDC, unsigned int nFlags) {
    if (pThis == nullptr) return 0;
    if ((nFlags & 0x8u) != 0) {
        ::SendMessage(pThis->m_hWnd, WM_ERASEBKGND,
                      reinterpret_cast<WPARAM>(pDC != nullptr ? pDC->GetSafeHdc() : nullptr), 0);
    }
    if ((nFlags & 0x4u) != 0) {
        impl__DoPaint_CBasePane__MEAAXPEAVCDC___Z(pThis, pDC);
    }
    return 0;
}

// CBasePane::OnSetAccData -- the export folded onto the shared
// `mov $0x1,%eax ; ret` body at RVA 0x3ae0, which CBasePane vftable slot 151
// points at, so the base implementation is `return TRUE;` and reads nothing.
// (Derived panes override it to fill m_AccData; the base class leaves the block
// as the caller found it.)
// Symbol: ?OnSetAccData@CBasePane@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CBasePane__UEAAHJ_Z(CBasePane* pThis, long lVal) {
    (void)pThis;
    (void)lVal;
    return TRUE;
}

// Symbol: ?OnSetIcon@CBasePane@@IEAAPEAUHICON__@@HPEAU2@@Z
extern "C" void* MS_ABI impl__OnSetIcon_CBasePane__IEAAPEAUHICON____HPEAU2__Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}

// Symbol: ?OnSettingChange@CBasePane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CBasePane__IEAAXIPEB_W_Z(unsigned int p0, const wchar_t* p1) {}

// Symbol: ?OnSize@CBasePane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CBasePane__IEAAXIHH_Z(unsigned int p0, int p1, int p2) {}

// Symbol: ?PaneFromPoint@CBasePane@@QEBAPEAV1@VCPoint@@H_NPEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__PaneFromPoint_CBasePane__QEBAPEAV1_VCPoint__H_NPEAUCRuntimeClass___Z(int p0, bool p1, void* /*struct*/* p2) {
    return nullptr;
}

// Symbol: ?PreTranslateMessage@CBasePane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CBasePane__UEAAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?RemovePaneFromDockManager@CBasePane@@QEAAXPEAV1@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CBasePane__QEAAXPEAV1_HHH0_Z(void* /*class*/* p0, int p1, int p2, int p3, void* /*class*/* p4) {}

// Symbol: ?SaveState@CBasePane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CBasePane__UEAAHPEB_WHI_Z(const wchar_t* p0, int p1, unsigned int p2) {
    return 0;
}

// Symbol: ?Serialize@CBasePane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CBasePane__UEAAXAEAVCArchive___Z(void* /*class*/* p0) {}

// Symbol: ?WindowProc@CBasePane@@MEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__WindowProc_CBasePane__MEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}
