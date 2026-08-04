// CMFCCustomizeButton — OpenMFC implementation.
// Sources: global_cmfccustomizebutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCustomizeButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CommonInit(): retail RVA 0x34fb0 -- zeroes 0x128/0x130/0x140/0x148 and sets
// 0x14c/0x150/0x158 to 1.  Transcribed verbatim.
// Symbol: ?CommonInit@CMFCCustomizeButton@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCCustomizeButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    s->m_nCommandID = 0;      // 0x128
    s->m_n148 = 0;            // 0x148
    s->m_b130 = 0;            // 0x130
    s->m_pToolbar = nullptr;  // 0x140
    s->m_n14c = 1;            // 0x14c
    s->m_n150 = 1;            // 0x150
    s->m_n158 = 1;            // 0x158
}
// CopyFrom(const CMFCToolBarButton& src): base copy, then the command id, the
// +0x138 caption string and the +0x148/+0x150/+0x158 ints.  Transcribed from
// retail RVA 0x36010.
// Symbol: ?CopyFrom@CMFCCustomizeButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCCustomizeButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& src)
{
    if (pThis == nullptr) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, &src);  // 0x180172ca0
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    const CZButton* srcB = reinterpret_cast<const CZButton*>(&src);
    s->m_nCommandID = srcB->m_nCommandID;               // 0x128
    if (srcB->m_str138.GetString() != nullptr) {
        s->m_str138 = srcB->m_str138;                   // CString copy (0x18000de30)
    }
    s->m_n148 = srcB->m_n148;                           // 0x148
    s->m_n150 = srcB->m_n150;                           // 0x150
    s->m_n158 = srcB->m_n158;                           // 0x158
}
// CreateObject(): retail (RVA 0x34d30) allocates 0x198 bytes and runs the
// constructor (0x180034da0 = CMFCToolBarMenuButton base ctor + CommonInit +
// the module-string +0x138/0x160 setup).  The opaque class has no modeled
// constructor or vftable, so nullptr is returned.
// Symbol: ?CreateObject@CMFCCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomizeButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x198 bytes
    // and runs the CMFCCustomizeButton constructor (0x180034da0, retail
    // vftable 0x1802e3688); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}
// CreatePopupMenu(): retail (RVA 0x35380) allocates a CMFCPopupMenu, fills it
// from the toolbar's button list and returns it.  The CMFCPopupMenu class and
// the toolbar button-list machinery are not modeled, so nullptr is returned.
// Symbol: ?CreatePopupMenu@CMFCCustomizeButton@@UEAAPEAVCMFCPopupMenu@@XZ
extern "C" void* MS_ABI impl__CreatePopupMenu_CMFCCustomizeButton__UEAAPEAVCMFCPopupMenu__XZ(
    void* pThis)
{
    if (pThis == nullptr) return nullptr;
    // TODO(clean-room): not transcribed -- the retail builds a CMFCPopupMenu
    // (0x19c8-byte allocation + ctor 0x1800b4cb0) and copies the toolbar
    // button list into it; the CMFCPopupMenu machinery is not modeled.
    return nullptr;
}
// InvokeCommand(CMFCPopupMenuBar* pMenuBar, const CMFCToolBarButton* pButton):
// invokes the clicked menu command.  Retail RVA 0x360d0 walks the toolbar's
// button list, checks IsCommandExist-style maps and posts WM_COMMAND.  The
// toolbar internals are not modeled, so FALSE is returned.
// Symbol: ?InvokeCommand@CMFCCustomizeButton@@UEAAHPEAVCMFCPopupMenuBar@@PEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__InvokeCommand_CMFCCustomizeButton__UEAAHPEAVCMFCPopupMenuBar__PEBVCMFCToolBarButton___Z(
    void* pThis, void* /*pMenuBar*/, const CMFCToolBarButton* /*pButton*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): not transcribed -- depends on the toolbar button
    // list at +0x168/+0x178, the CMFCToolBar vtable calls (0x18014f000 /
    // 0x18014fe00) and the module-global command maps; not modeled.
    return 0;
}
// OnCalculateSize(CDC* pDC, CSize sizeButton, BOOL bHorz): returns the button
// size.  Retail RVA 0x35ee0 walks the +0x38 caption string / toolbar state.
// The hidden return slot is the second parameter (after `this`), matching the
// retail register usage.  Conservative: returns {0, 0}.
// Symbol: ?OnCalculateSize@CMFCCustomizeButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCCustomizeButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, void* /*pDC*/, const CSize* /*sizeButton*/, int /*bHorz*/)
{
    if (pThis == nullptr || pRet == nullptr) return pRet;
    // TODO(clean-room): not transcribed -- the retail depends on the toolbar
    // button list, CMFCToolBar::GetButtonSize-style globals and the caption
    // CString at +0x38; not modeled.
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}
// OnCancelMode(): base handler plus toolbar-button invalidation while
// displayed.  Transcribed from retail RVA 0x35080 (the base
// CMFCToolBarButton::OnCancelMode at 0x180175820 is not modeled).
// Symbol: ?OnCancelMode@CMFCCustomizeButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCCustomizeButton__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- the base
    // CMFCToolBarButton::OnCancelMode (0x180175820) is not modeled.
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    if ((s->m_b12c != 0 || s->m_b130 != 0) && s->m_pToolbar != nullptr) {
        CMFCToolBar* pToolbar = reinterpret_cast<CMFCToolBar*>(s->m_pToolbar);
        // Retail: CMFCToolBar::ButtonToIndex (0x18014f000) then
        // InvalidateButton (0x18014fe50).
        int idx = -1;
        if (pToolbar != nullptr) {
            for (int i = 0; i < pToolbar->GetCount(); i++) {
                if (pToolbar->GetButton(i) == reinterpret_cast<CMFCToolBarButton*>(pThis)) {
                    idx = i;
                    break;
                }
            }
        }
        if (idx >= 0) {
            (void)impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
                pToolbar, idx);
        }
    }
}
// OnChangeParentWnd(CWnd* pWndParent): stores the new parent, resets the
// toolbar link and the "parent is a toolbar container" flag.  Transcribed from
// retail RVA 0x34050 (the base CMFCToolBarMenuButton::OnChangeParentWnd at
// 0x18015e570 and the two IsKindOf class checks are not modeled).
// Symbol: ?OnChangeParentWnd@CMFCCustomizeButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCCustomizeButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWndParent)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- the base
    // CMFCToolBarMenuButton::OnChangeParentWnd (0x18015e570) is not modeled.
    CZButton* s = reinterpret_cast<CZButton*>(pThis);
    // Retail: +0x140 keeps pWndParent only when it IsKindOf's the runtime
    // class at 0x1803b15f8 (the toolbar).  The class is unidentifiable, so the
    // CMFCToolBar test stands in for it.
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
        // Retail re-checks the parent's class (rtc 0x180323260) before setting
        // +0x154; keep the FromHandle result.
        s->m_b154 = (pFrom != nullptr) ? 1 : 0;
    }
}
// OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, ...): draws
// the "customize" button.  Retail RVA 0x350f0 uses the base
// CMFCToolBarMenuButton::OnDraw (0x18015e300), the toolbar image state and the
// caption string; none of that is modeled, so the body is conservative.
// Symbol: ?OnDraw@CMFCCustomizeButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCustomizeButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* /*pDC*/, const CRect* /*rect*/,
    void* /*pImages*/, int /*bLarge*/, int /*bHilite*/, int /*bText*/,
    int /*bCenter*/, int /*bShow1*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): not transcribed -- depends on the base OnDraw
    // (0x18015e300), the toolbar image list and the visual-manager drawing
    // helpers; not modeled.
}
// Symbol: ??0CMFCCustomizeButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCCustomizeButton@@QEAA@HAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void* MS_ABI impl___0CMFCCustomizeButton__QEAA_HAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, int flags, const void* text) {
    (void)flags;
    (void)text;
    return pThis;
}
