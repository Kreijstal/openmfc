// CMFCCustomizeButton — OpenMFC implementation.
// Sources: global_cmfccustomizebutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCustomizeButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" int MS_ABI impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(
    const CMFCToolBar* pThis, const CMFCToolBarButton* pButton);

// CommonInit(): retail mfc140u RVA 0x34fb0 zeroes the qword at +0x128,
// the dwords at +0x130/+0x148, and the qwords at +0x140/+0x150; it sets
// +0x14c/+0x150/+0x158 to 1.  The qword stores also clear +0x12c/+0x154.
// Transcribed from the retail body.
// Symbol: ?CommonInit@CMFCCustomizeButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCCustomizeButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    s->m_nCommandID = 0;      // 0x128
    s->m_b12c = 0;            // 0x12c, cleared by retail's qword store at +0x128
    s->m_b130 = 0;            // 0x130
    s->m_n148 = 0;            // 0x148
    s->m_n14c = 1;            // 0x14c
    s->m_n150 = 1;            // 0x150
    s->m_b154 = 0;            // 0x154, cleared by retail's qword store at +0x150
    s->m_n158 = 1;            // 0x158
    s->m_pToolbar = nullptr;  // 0x140
}
// CopyFrom(const CMFCToolBarButton& src): retail mfc140u RVA 0x36010 calls
// the base copy, then copies the command id, +0x138 CString and
// +0x148/+0x150/+0x158.  The CString assignment is unconditional in retail.
// Symbol: ?CopyFrom@CMFCCustomizeButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCCustomizeButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& src)
{
    if (pThis == nullptr) return;
    impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
        reinterpret_cast<CMFCToolBarMenuButton*>(pThis), &src);  // mfc140u RVA 0x172ca0
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    const CZButton* srcB = reinterpret_cast<const CZButton*>(&src);
    s->m_nCommandID = srcB->m_nCommandID;               // 0x128
    s->m_str138 = srcB->m_str138;                       // includes the empty-source case
    s->m_n148 = srcB->m_n148;                           // 0x148
    s->m_n150 = srcB->m_n150;                           // 0x150
    s->m_n158 = srcB->m_n158;                           // 0x158
}
// CreateObject(): retail mfc140u RVA 0x34d30 allocates 0x198 bytes and, on
// success, transfers to the constructor at RVA 0x34da0 (mfc140u).  This remains a stub:
// the local constructor thunk is a placeholder and no usable derived vftable
// is modeled, so returning allocated storage would be unsafe.
// Symbol: ?CreateObject@CMFCCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomizeButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): intentionally left stubbed.  Retail's constructor at
    // mfc140u RVA 0x34da0 initializes the base, derived vftable and CString-
    // backed state; the local constructor thunk below only returns pThis.
    // Returning nullptr therefore differs from retail when allocation succeeds.
    return nullptr;
}
// CreatePopupMenu(): retail mfc140u RVA 0x35380 builds a CMFCPopupMenu
// (0x19c8-byte allocation, constructor at RVA 0xb4cb0 (mfc140u)), then populates it from
// toolbar buttons and command state.  The classes exist locally, but this
// large menu-building path is not faithfully transcribed; this remains a stub.
// Symbol: ?CreatePopupMenu@CMFCCustomizeButton@@UEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_CMFCCustomizeButton__UEAAPEAVCMFCPopupMenu__XZ(
    void* pThis)
{
    if (pThis == nullptr) return nullptr;
    // TODO(clean-room): not transcribed -- the retail path depends on the
    // toolbar's button list, menu state and command maps; returning nullptr
    // differs from retail whenever popup construction succeeds.
    return nullptr;
}
// InvokeCommand(CMFCPopupMenuBar* pMenuBar, const CMFCToolBarButton* pButton):
// retail mfc140u RVA 0x360d0 locates the button, checks toolbar/global command
// state, and posts WM_COMMAND on success.  This command-map path is not
// reconstructed here, so the body remains a stub.
// Symbol: ?InvokeCommand@CMFCCustomizeButton@@UEAAHPEAVCMFCPopupMenuBar@@PEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__InvokeCommand_CMFCCustomizeButton__UEAAHPEAVCMFCPopupMenuBar__PEBVCMFCToolBarButton___Z(
    void* pThis, void* /*pMenuBar*/, const CMFCToolBarButton* /*pButton*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): not transcribed -- retail traverses the list at
    // +0x168/+0x178, checks toolbar virtual state and consults global command
    // maps before posting WM_COMMAND.  The exact command-state path is missing.
    return 0;
}
// OnCalculateSize(CDC* pDC, CSize sizeButton, BOOL bHorz): returns the button
// size.  Retail RVA 0x35ee0 (mfc140u) uses the +0x38 caption string and toolbar state.
// The hidden return slot is the second parameter (after `this`), matching the
// retail register usage.  Conservative: returns {0, 0}.
// Symbol: ?OnCalculateSize@CMFCCustomizeButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCCustomizeButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, void* /*pDC*/, const CSize* /*sizeButton*/, int /*bHorz*/)
{
    if (pThis == nullptr || pRet == nullptr) return pRet;
    // TODO(clean-room): not transcribed -- retail branches on +0x148, may load
    // resource 0x4279 into the +0x38 CString, and uses toolbar/global metrics
    // to calculate orientation- and DPI-dependent dimensions.
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}
// OnCancelMode(): retail RVA 0x36080 (mfc140u) calls the base
// CMFCToolBarMenuButton::OnCancelMode (entry RVA 0x175820 (mfc140u)), then invalidates
// the toolbar button when +0x12c or +0x130 is set.
// Symbol: ?OnCancelMode@CMFCCustomizeButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCCustomizeButton__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): partial approximation; the retail base handler at
    // RVA 0x175820 (mfc140u) has no local implementation, so its side effects
    // are omitted.  The local ButtonToIndex helper replaces the retail scan.
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    if ((s->m_b12c != 0 || s->m_b130 != 0) && s->m_pToolbar != nullptr) {
        CMFCToolBar* pToolbar = reinterpret_cast<CMFCToolBar*>(s->m_pToolbar);
        // Retail calls ButtonToIndex at RVA 0x14f000 (mfc140u), then
        // InvalidateButton at RVA 0x14fe50 (mfc140u).
        const int idx = impl__ButtonToIndex_CMFCToolBar__QEBAHPEBVCMFCToolBarButton___Z(
            pToolbar, reinterpret_cast<const CMFCToolBarButton*>(pThis));
        if (idx >= 0) {
            (void)impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
                pToolbar, idx);
        }
    }
}
// OnChangeParentWnd(CWnd* pWndParent): retail RVA 0x35050 (mfc140u) stores the
// new parent, retains it as a toolbar only when it IsKindOf(CMFCToolBar), and
// sets +0x154 only when the parent's parent is a CReBar.  The base
// CMFCToolBarButton::OnChangeParentWnd is at RVA 0x15e570 (mfc140u); its local
// thunk is a placeholder, so the base side effects are omitted here.
// Symbol: ?OnChangeParentWnd@CMFCCustomizeButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCCustomizeButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWndParent)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): partial approximation; the base
    // CMFCToolBarButton::OnChangeParentWnd thunk is only a placeholder locally.
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    // Retail's +0x140 check is IsKindOf(CMFCToolBar); this runtime-class
    // getter supplies the corresponding local descriptor.
    if (pWndParent != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            reinterpret_cast<CObject*>(pWndParent),
            impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        s->m_pToolbar = pWndParent;
    } else {
        s->m_pToolbar = nullptr;
    }
    reinterpret_cast<CMFCToolBarButton*>(pThis)->m_pWndParent = pWndParent;  // +0x80
    reinterpret_cast<CMFCToolBarButton*>(pThis)->m_bText = 0;                // +0xc
    s->m_n148 = 0;
    s->m_b154 = 0;
    if (pWndParent != nullptr) {
        HWND hwndParent = ::GetParent(pWndParent->m_hWnd);
        CWnd* pFrom = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndParent);
        // Retail sets +0x154 only when this parent IsKindOf(CReBar).
        s->m_b154 = (pFrom != nullptr &&
            impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
                reinterpret_cast<CObject*>(pFrom),
                impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ())) ? 1 : 0;
    }
}
// OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, ...): the
// retail entry at RVA 0x350f0 (mfc140u) calls CMFCToolBarButton::FillInterior
// (entry RVA 0x15e300 (mfc140u)), then performs toolbar-image and visual-manager drawing.
// This body remains a no-op stub; the full rendering path is not transcribed.
// Symbol: ?OnDraw@CMFCCustomizeButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCustomizeButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* /*pDC*/, const CRect* /*rect*/,
    void* /*pImages*/, int /*bLarge*/, int /*bHilite*/, int /*bText*/,
    int /*bCenter*/, int /*bShow1*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): not transcribed -- the retail FillInterior call,
    // toolbar-image state and visual-manager rendering are not reproduced.
}
// Symbol: ??1CMFCCustomizeButton@@UEAA@XZ
// Stub: retail mfc140u RVA 0x34ff0 releases the +0x138 CString, tears down
// embedded state, and invokes the base destructor; those ownership effects
// are not modeled here.
extern "C" void MS_ABI impl___1CMFCCustomizeButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}
// Stub: retail mfc140u RVA 0x34da0 initializes the base, both vtables and
// derived fields; no usable derived vftable is modeled locally.
// Symbol: ??0CMFCCustomizeButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Stub: retail mfc140u RVA 0x34ec0 initializes the same state and assigns
// the command id and caption; this local thunk currently ignores both args.
// Symbol: ??0CMFCCustomizeButton@@QEAA@HAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_HAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, int flags, const void* text) {
    (void)flags;
    (void)text;
    return pThis;
}
