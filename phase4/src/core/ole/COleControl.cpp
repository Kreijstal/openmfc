// COleControl — OpenMFC implementation.
// Sources: global_ole-5_impl.cpp, global_ole-6_impl.cpp, global_olecontrol_batch1.cpp, global_olecontrol_batch2.cpp, manual_small_stub_implementations.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleControlSupport.h"
#include "detail/Ole6ImplSupport.h"
#include "detail/OlecontrolBatch2Support.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/OlecoreSupport.h"

// COleControl methods that the corrected retail member layout makes
// implementable for real. Every function here was decoded from the retail
// mfc140u export (ordinal -> RVA -> objdump) rather than inferred, and the
// member offsets the disassembly uses are exactly the ones transcribed into
// include/openmfc/afxole.h (see the static_asserts in olecore.cpp).
//
// The retail bitfield word lives at this+0x160; bit positions referenced below
// are the ones the shipping code actually tests:
//   bit  2 = m_bModified, bit 10 = m_bInPlaceActive.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif





// COleControl::AmbientScaleUnits() — retail (0x1ee9e0):
//     _AtlInstallStringThunk; *pRet = nil-string
//     GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, pRet)
//     return pRet
// RDX carries the hidden return slot for the by-value CString; the retail
// writes the result straight into it via the ambient lookup. The OpenMFC C++
// member AmbientScaleUnits() performs exactly that lookup.
// Symbol: ?AmbientScaleUnits@COleControl@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* MS_ABI
impl__AmbientScaleUnits_COleControl__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    COleControl* pThis, void* pRet)
{
    if (!pThis || !pRet) return nullptr;
    new (pRet) CString(pThis->AmbientScaleUnits());
    return pRet;
}
// COleControl::DoClick() — retail (0x1e1b50) dispatches through the virtual
// OnClick(1) (vtable slot 0x340); MFC's OnClick simply fires the Click event.
// The header does not declare OnClick, so the event fire is done directly.
// Symbol: ?DoClick@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__DoClick_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    pThis->FireClick();
}
// COleControl::FireError(SCODE, LPCTSTR, UINT) — retail (0x1e4420) fires the
// stock Error event (DISPID_ERROREVENT) after resolving the description /
// help-file strings. Only the four documented Error-event parameters are
// carried by OpenMFC's FireEvent (retail additionally pushes internal help
// strings and a retval slot that the event contract does not expose).
// Symbol: ?FireError@COleControl@@QEAAXJPEB_WI@Z
extern "C" void MS_ABI impl__FireError_COleControl__QEAAXJPEB_WI_Z(
    COleControl* pThis, long scode, const wchar_t* lpszDescription, unsigned int nHelpID)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially.
    BYTE params[] = { VT_I4, VT_BSTR, VT_BSTR, VT_I4, 0 };
    pThis->FireEvent(DISPID_ERROREVENT, params,
                     scode,
                     lpszDescription ? lpszDescription : L"",
                     static_cast<const wchar_t*>(nullptr),
                     static_cast<long>(nHelpID));
}
// COleControl::ButtonDown(USHORT, UINT, CPoint) — retail (0x1e3dd0):
//     if (button-message || m_bPendingUIActivation) {
//         if (m_iButtonState == 0) SetCapture();
//         m_iButtonState = nButton;                // bits 4-6 of the 0x160 word
//         if (message & 0x20) FireEvent(MOUSEDOWN, nButton, shift, x, y);
//         m_iDblClkState &= ~nButton;              // bits 7-9 cleared
//     }
// The message-gating (which the retail derives from the current MSG) is not
// reproducible without the message window; firing is what MFC documents.
// Symbol: ?ButtonDown@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (pThis->m_iButtonState == 0) pThis->SetCapture();
    pThis->m_iButtonState = nButton & 7;
    pThis->m_iDblClkState &= ~(nButton & 7);
    pThis->FireMouseDown((short)nButton, (short)nShiftState, PtX(pt), PtY(pt));
}
// COleControl::ButtonDblClk(USHORT, UINT, CPoint) — retail (0x1e3fd0):
//     if (button-message) {
//         SetCapture();
//         m_iButtonState = nButton;
//         if (message & 0x2) FireEvent(DISPID_DBLCLICK, no params);
//         m_iDblClkState = nButton;                // bits 7-9
//     }
// Symbol: ?ButtonDblClk@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int /*nShiftState*/,
    unsigned long long /*pt*/)
{
    if (!pThis) return;
    pThis->SetCapture();
    pThis->m_iButtonState = nButton & 7;
    pThis->FireDblClick();
    pThis->m_iDblClkState = nButton & 7;
}
// COleControl::ButtonUp(USHORT, UINT, CPoint) — retail (0x1e3e90):
//     if (m_iButtonState == 0) return;
//     m_iButtonState &= ~nButton;
//     if (m_iButtonState == 0) ReleaseCapture();
//     if (message & 0x80) FireEvent(MOUSEUP, nButton, shift, x, y);
//     if ((message & 0x1) && !(m_iDblClkState & nButton) && PtInRect(rc, pt))
//         OnClick(nButton);                        // fires DISPID_CLICK
//     m_iDblClkState &= ~nButton;
// The retail's rect-inside test uses a virtual (vtable slot 0x2d8) that the
// headers do not model; the click fire is kept unconditionally.
// Symbol: ?ButtonUp@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (pThis->m_iButtonState == 0) return;
    pThis->m_iButtonState &= ~(nButton & 7);
    if (pThis->m_iButtonState == 0) pThis->ReleaseCapture();
    pThis->FireMouseUp((short)nButton, (short)nShiftState, PtX(pt), PtY(pt));
    if ((pThis->m_iDblClkState & (nButton & 7)) == 0) {
        pThis->FireClick();
    }
    pThis->m_iDblClkState &= ~(nButton & 7);
}
// COleControl::GetClientOffset(long*, long*) const — retail (0x1e6060):
//     n = (m_sBorderStyle == 1) + (m_sAppearance == 1 ? 2 : 0);
//     if (!n) { *pDx = 0; *pDy = 0; }
//     else { *pDx = GetSystemMetrics(SM_CXBORDER) * n;
//            *pDy = GetSystemMetrics(SM_CYBORDER) * n; }
// Symbol: ?GetClientOffset@COleControl@@UEBAXPEAJ0@Z
extern "C" void MS_ABI impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(
    const COleControl* pThis, long* pDx, long* pDy)
{
    if (!pThis || !pDx || !pDy) return;
    const long n = (pThis->m_sBorderStyle == 1) + (pThis->m_sAppearance == 1 ? 2 : 0);
    if (n != 0) {
        *pDx = ::GetSystemMetrics(SM_CXBORDER) * n;
        *pDy = ::GetSystemMetrics(SM_CYBORDER) * n;
    } else {
        *pDx = 0;
        *pDy = 0;
    }
}
// COleControl::ClientToParent(LPCRECT, LPPOINT) const — retail (0x1e61c0):
//     GetClientOffset(&offX, &offY);               // virtual slot 0x4a8
//     pt->x += lpRect->left + offX;
//     pt->y += lpRect->top  + offY;
// Symbol: ?ClientToParent@COleControl@@UEBAXPEBUtagRECT@@PEAUtagPOINT@@@Z
extern "C" void MS_ABI impl__ClientToParent_COleControl__UEBAXPEBUtagRECT__PEAUtagPOINT___Z(
    const COleControl* pThis, const RECT* lpRect, POINT* lpPoint)
{
    if (!pThis || !lpRect || !lpPoint) return;
    long offX = 0, offY = 0;
    // Retail dispatches through the virtual GetClientOffset; the base-class
    // implementation (this file) is the OpenMFC one.
    impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(pThis, &offX, &offY);
    lpPoint->x += lpRect->left + offX;
    lpPoint->y += lpRect->top + offY;
}
// COleControl::GetClientRect(LPRECT) const — retail (0x1e6c00):
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless) {
//         GetClientOffset(&offX, &offY);           // virtual slot 0x4a8
//         *lpRect = m_rcPos;
//         OffsetRect(lpRect, -offX, -offY);
//         OffsetRect(lpRect, -lpRect->left, -lpRect->top);   // origin (0,0)
//     } else if (m_hWnd) GetClientRect(m_hWnd, lpRect);
//     else SetRectEmpty(lpRect);
// The two offsets cancel out of the origin normalization, leaving the control's
// own (0,0,width,height) client rect.
// Symbol: ?GetClientRect@COleControl@@UEBAXPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetClientRect_COleControl__UEBAXPEAUtagRECT___Z(
    const COleControl* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        long offX = 0, offY = 0;
        impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(pThis, &offX, &offY);
        RECT rc = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                    pThis->m_rcPos.right, pThis->m_rcPos.bottom };
        ::OffsetRect(&rc, -offX, -offY);
        ::OffsetRect(&rc, -rc.left, -rc.top);
        *lpRect = rc;
    } else if (pThis->m_hWnd) {
        ::GetClientRect(pThis->m_hWnd, lpRect);
    } else {
        ::SetRectEmpty(lpRect);
    }
}
// COleControl::ClipCaretRect(LPRECT) — retail (0x1e6b20):
//     if (!(m_bInPlaceActive && m_bInPlaceSiteWndless)) return FALSE;
//     GetClippingRect(&m_rcPos, &clip);            // virtual slot 0x4b8
//     rect = *lpRect; OffsetRect(rect, -rect.left, -rect.top);
//     hr = m_pInPlaceSite->OnDefWindowMessage(WM_CARET_CLIPRECT, 0, (LPARAM)&rect, 0);
//     if (SUCCEEDED(hr)) { OffsetRect(rect, -rect.left, -rect.top); *lpRect = rect; }
//     return SUCCEEDED(hr);
// Symbol: ?ClipCaretRect@COleControl@@QEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__ClipCaretRect_COleControl__QEAAHPEAUtagRECT___Z(
    COleControl* pThis, RECT* lpRect)
{
    if (!pThis || !lpRect) return FALSE;
    if (!(pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless)) return FALSE;
    // TODO(clean-room): transcribed partially. The container's windowless site
    // must clip the caret via the private WM_CARET_CLIPRECT path; that site
    // vtable is not modeled in OpenMFC.
    return FALSE;
}
// COleControl::GetCapture() — retail (0x1e6630):
//     if (m_bInPlaceActive && m_bInPlaceSiteWndless)
//         return m_pInPlaceSite->GetCapture() == S_OK ? this : NULL;
//     return CWnd::FromHandle(::GetCapture());
// (IOleInPlaceSiteWindowless::GetCapture takes no arguments and returns S_OK
// when the control holds the capture.)
// Symbol: ?GetCapture@COleControl@@QEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetCapture_COleControl__QEAAPEAVCWnd__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
        void* pSite = pThis->m_pInPlaceSite;
        if (!pSite) return nullptr;
        // IOleInPlaceSiteWindowless::GetCapture is slot 19 of the site vtable.
        void** vft = *static_cast<void***>(pSite);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*)>(vft[19]);
        return fn(pSite) == S_OK ? pThis : nullptr;
    }
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture());
}
// COleControl::DefWindowProcW — retail (0x1e2130):
//     if (m_hWnd) goto CWnd::DefWindowProcW;       // shared implementation
//     return 0;
// Symbol: ?DefWindowProcW@COleControl@@MEAA_JI_K_J@Z
extern "C" LRESULT MS_ABI impl__DefWindowProcW_COleControl__MEAA_JI_K_J_Z(
    COleControl* pThis, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!pThis || !pThis->m_hWnd) return 0;
    return impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(pThis, message, wParam, lParam);
}
// COleControl::ForwardActivationMsg(MSG*) — retail (0x1e51f0):
//     if (m_bInPlaceSiteWndless && message in 0x200..0x209)
//         translate via the wndless site, then dispatch to a virtual (slot 0x498);
//     else ::SendMessageW(m_hWnd, msg, wParam, lParam);
// Symbol: ?ForwardActivationMsg@COleControl@@IEAAXPEAUtagMSG@@@Z
extern "C" void MS_ABI impl__ForwardActivationMsg_COleControl__IEAAXPEAUtagMSG___Z(
    COleControl* pThis, MSG* pMsg)
{
    if (!pThis || !pMsg) return;
    // TODO(clean-room): transcribed partially. The windowless translation path
    // (site GetWindow + OnDefWindowMessage + virtual dispatch) is not modeled;
    // the windowed path forwards the raw message, which is what a non-windowless
    // control observes in retail.
    if (pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
    }
}
// COleControl::CreateWindowForSubclassedControl() — retail (0x1e1cd0):
//     if (!PreCreateWindow()) return;              // virtual slot 0x468
//     if (m_hWnd) return;
//     cls = GetWindowClass();                      // unexported helper
//     if (!cls) return;
//     size = HIMETRICtoPixel-ish(m_cxExtent, m_cyExtent);   // helper 0x26080c
//     CreateEx(...);                               // virtual slot 0x4c0
// Symbol: ?CreateWindowForSubclassedControl@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(
    COleControl* /*pThis*/)
{
    // TODO(clean-room): transcribed partially. Requires the superclass window
    // class registration and unit-conversion helpers OpenMFC does not provide;
    // a no-op is type-correct (void) and leaves m_hWnd untouched.
}
// COleControl::DoSuperclassPaint(CDC*, const CRect&) — retail (0x1dfd70):
//     if (!m_hWnd) CreateWindowForSubclassedControl();
//     if (!m_hWnd) return;
//     ... compare control rect with window rect, then paint through the
//     superclass WNDPROC via CallWindowProcW ...
// Symbol: ?DoSuperclassPaint@COleControl@@QEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DoSuperclassPaint_COleControl__QEAAXPEAVCDC__AEBVCRect___Z(
    COleControl* pThis, CDC* pDC, const CRect& /*rcBounds*/)
{
    if (!pThis) return;
    if (!pThis->m_hWnd) {
        impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(pThis);
    }
    // TODO(clean-room): transcribed partially. Painting through the superclass
    // window proc (CallWindowProcW) is not reproducible without a registered
    // superclass WNDPROC.
    (void)pDC;
}
// COleControl::CreateFrameWindow() — retail (0x1e0f80):
//     wnd = new CControlFrameWnd(this);            // 0xf0 bytes, ctor 0x1802b29d0
//     if (wnd && wnd->Create(...)) return wnd;     // virtual slot 0x2d8
//     return NULL;
// The OpenMFC CControlFrameWnd ctor stub installs no vtable, so running the
// virtual Create would read uninitialized memory; report failure instead.
// Symbol: ?CreateFrameWindow@COleControl@@MEAAPEAVCControlFrameWnd@@XZ
extern "C" void* MS_ABI impl__CreateFrameWindow_COleControl__MEAAPEAVCControlFrameWnd__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    // TODO(clean-room): transcribed partially.
    return nullptr;
}
// COleControl::CreateControlWindow(HWND, const CRect&, LPCRECT) — retail
// (0x1dff10) computes the window style from m_sBorderStyle/m_sAppearance/
// m_bEnabled, creates a CControlFrameWnd (or reuses m_pReflect), creates the
// HWND and reports m_hWnd != 0.
// Symbol: ?CreateControlWindow@COleControl@@MEAAHPEAUHWND__@@AEBVCRect@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__CreateControlWindow_COleControl__MEAAHPEAUHWND____AEBVCRect__PEBUtagRECT___Z(
    COleControl* pThis, HWND /*hwndParent*/, const CRect& /*rcPos*/, const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. The frame-window creation path
    // is not implemented (see CreateFrameWindow); a window already attached is
    // the only success case.
    return pThis->m_hWnd ? TRUE : FALSE;
}
// COleControl::DrawContent(CDC*, CRect&) — retail (0x1e0420):
//     if (!m_bOptimizedDraw) nSaveDC = pDC->SaveDC();
//     ... SetBkMode(TRANSPARENT) ...; m_rcBounds = rc;
//     OnDraw(pDC, rc, rc);                         // virtual slot 0x2f0
//     if (!m_bOptimizedDraw && nSaveDC) pDC->RestoreDC(nSaveDC);
// Symbol: ?DrawContent@COleControl@@IEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawContent_COleControl__IEAAXPEAVCDC__AEAVCRect___Z(
    COleControl* pThis, CDC* pDC, CRect& rc)
{
    if (!pThis || !pDC) return;
    pThis->m_rcBounds = rc;
    pThis->OnDraw(pDC, rc, rc);
}
// COleControl::DrawMetafile(CDC*, CRect&) — retail (0x1e0550):
//     m_rcBounds = rc;  OnDrawMetafile(pDC, rc);   // virtual slot 0x2f8
// Symbol: ?DrawMetafile@COleControl@@IEAAXPEAVCDC@@AEAVCRect@@@Z
extern "C" void MS_ABI impl__DrawMetafile_COleControl__IEAAXPEAVCDC__AEAVCRect___Z(
    COleControl* pThis, CDC* pDC, CRect& rc)
{
    if (!pThis || !pDC) return;
    pThis->m_rcBounds = rc;
    // TODO(clean-room): transcribed partially. Retail dispatches to the virtual
    // OnDrawMetafile which the header does not model; fall back to OnDraw so the
    // control still renders.
    pThis->OnDraw(pDC, rc, rc);
}
// COleControl::ExchangeExtent(CPropExchange*) — retail (0x1ef880) exchanges
// m_cxExtent/m_cyExtent with the "Extent" properties through the PX_* helpers
// and reports changes to IOleObject::SetExtent(DVASPECT_CONTENT, size).
// Symbol: ?ExchangeExtent@COleControl@@QEAAHPEAVCPropExchange@@@Z
extern "C" int MS_ABI impl__ExchangeExtent_COleControl__QEAAHPEAVCPropExchange___Z(
    COleControl* pThis, CPropExchange* pPX)
{
    if (!pThis || !pPX) return FALSE;
    // TODO(clean-room): transcribed partially. The PX_* exchange helpers are
    // not exported by OpenMFC; report a successful (no-op) exchange.
    return TRUE;
}
// COleControl::ExchangeStockProps(CPropExchange*) — retail (0x1ecfa0) is a
// large routine that exchanges all stock properties (Appearance, BorderStyle,
// BackColor, ForeColor, Font, Caption, Enabled, ReadyState) in one pass.
// Symbol: ?ExchangeStockProps@COleControl@@QEAAXPEAVCPropExchange@@@Z
extern "C" void MS_ABI impl__ExchangeStockProps_COleControl__QEAAXPEAVCPropExchange___Z(
    COleControl* /*pThis*/, CPropExchange* /*pPX*/)
{
    // TODO(clean-room): transcribed partially. The per-property PX_* helpers
    // are not exported; a no-op is type-correct (void).
}
// COleControl::BuildSharedMenu() — retail (0x1e52f0) allocates a 0x28-byte
// OLE menu-holder at m_pUIActiveInfo and shares the in-place frame's menu into
// it. The holder type is not modeled in afxole.h, so only the already-built
// case can be reproduced.
// Symbol: ?BuildSharedMenu@COleControl@@MEAAHXZ
extern "C" int MS_ABI impl__BuildSharedMenu_COleControl__MEAAHXZ(COleControl* pThis)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially.
    return pThis->m_pUIActiveInfo != nullptr ? TRUE : FALSE;
}
// COleControl::DestroySharedMenu() — retail (0x1e5430) releases the shared
// menu held by the m_pUIActiveInfo holder and frees the 0x28-byte object.
// Symbol: ?DestroySharedMenu@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__DestroySharedMenu_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. The holder object was never
    // allocated by the OpenMFC BuildSharedMenu, so dropping the pointer is the
    // symmetric no-op.
    pThis->m_pUIActiveInfo = nullptr;
}
// COleControl::ControlInfoChanged() — retail (0x1e1700):
//     if (m_pControlSite) m_pControlSite->OnControlInfoChanged();
// (OnControlInfoChanged is slot 3 of the IOleControlSite vtable.)
// Symbol: ?ControlInfoChanged@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__ControlInfoChanged_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    void* pSite = pThis->m_pControlSite;
    if (!pSite) return;
    void** vft = *static_cast<void***>(pSite);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*)>(vft[3]);
    fn(pSite);
}
// COleControl::DisplayError(SCODE, LPCTSTR, LPCTSTR, DWORD, UINT) — retail
// (0x1e45b0) formats the error text and shows it with a message box; it
// mutates nothing on the control, so a no-op is type-correct and safe.
// Symbol: ?DisplayError@COleControl@@UEAAXJPEB_W00I@Z
extern "C" void MS_ABI impl__DisplayError_COleControl__UEAAXJPEB_W00I_Z(
    COleControl* /*pThis*/, long /*scode*/, const wchar_t* /*lpszDescription*/,
    long /*lpszHelpFile*/, long /*dwHelpContext*/, unsigned int /*nHelpID*/)
{
    // TODO(clean-room): transcribed partially. The message-box UI is skipped.
}
// COleControl::CreateTracker(int, int) — retail (0x1f5510) zeroes the RECT
// pointer and tail-calls the 3-argument form (default rect = m_rcPos).
// Symbol: ?CreateTracker@COleControl@@IEAAXHH@Z
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHH_Z(
    COleControl* pThis, int nSize, int bEnableResize)
{
    if (!pThis) return;
    impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(pThis, nSize, bEnableResize, nullptr);
}
// COleControl::CreateTracker(int, int, LPCRECT) — retail (0x1f5520):
//     style  = (nSize ? 0x10 : 0x0) | (bEnableResize ? 0x4 : 0x0);
//     rect   = m_rcPos (adjusted by an internal helper);
//     m_pRectTracker = new CRectTracker(rect, style);   // 0x60-byte object
//     if (lpRect) CopyRect(&tracker->tail, lpRect);
//     OnSetObjectRects(&m_rcPos, lpRect ? lpRect : &m_rcPos);   // virtual slot 0x330
// Symbol: ?CreateTracker@COleControl@@IEAAXHHPEBUtagRECT@@@Z
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(
    COleControl* pThis, int nSize, int bEnableResize, const RECT* lpRect)
{
    if (!pThis) return;
    const unsigned int style = (nSize != 0 ? 0x10u : 0x0u) |
                               (bEnableResize != 0 ? 0x4u : 0x0u);
    void* pTracker = ::operator new(0x60);
    if (pTracker) {
        RECT rc = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                    pThis->m_rcPos.right, pThis->m_rcPos.bottom };
        impl___0CRectTracker__QEAA_PEBUtagRECT__I_Z(pTracker, &rc, style);
    }
    pThis->m_pRectTracker = pTracker;
    const RECT rcPos = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                         pThis->m_rcPos.right, pThis->m_rcPos.bottom };
    const RECT* notifyRect = lpRect ? lpRect : &rcPos;
    pThis->OnSetObjectRects(&rcPos, notifyRect);
}
// COleControl::DestroyTracker() — retail (0x1f5620):
//     if (m_pRectTracker) { delete m_pRectTracker; m_pRectTracker = NULL; }
//     OnSetObjectRects(&m_rcPos, &tracker-tail-state);   // virtual slot 0x330
// Symbol: ?DestroyTracker@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__DestroyTracker_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    void* pTracker = pThis->m_pRectTracker;
    if (pTracker) {
        impl___1CRectTracker__UEAA_XZ(pTracker);
        ::operator delete(pTracker);
    }
    pThis->m_pRectTracker = nullptr;
    // TODO(clean-room): retail forwards the tracker's tail rect state here;
    // m_rcPos is the closest named equivalent.
    const RECT rcPos = { pThis->m_rcPos.left, pThis->m_rcPos.top,
                         pThis->m_rcPos.right, pThis->m_rcPos.bottom };
    pThis->OnSetObjectRects(&rcPos, &rcPos);
}
// COleControl::GetConnectionMap() const — retail returns the address of a
// static, entry-less AFX_CONNECTIONMAP. The type is not modeled in OpenMFC and
// the base control declares no connection-map entries, so a null pointer is the
// safe base-class default (matching GetPropPageIDs' behavior).
// Symbol: ?GetConnectionMap@COleControl@@MEBAPEBUAFX_CONNECTIONMAP@@XZ
extern "C" const void* MS_ABI impl__GetConnectionMap_COleControl__MEBAPEBUAFX_CONNECTIONMAP__XZ(
    const COleControl* /*pThis*/)
{
    // TODO(clean-room): transcribed partially.
    return nullptr;
}
// COleControl::GetConnectionHook(const IID&) — retail (0x1df000):
//     if (!m_piidEvents) return NULL;
//     return memcmp(piid, m_piidEvents, 16) == 0 ? &m_xPropConnPt : NULL;
// Symbol: ?GetConnectionHook@COleControl@@MEAAPEAUIConnectionPoint@@AEBU_GUID@@@Z
extern "C" IConnectionPoint* MS_ABI
impl__GetConnectionHook_COleControl__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(
    COleControl* pThis, const GUID& iid)
{
    if (!pThis || !pThis->m_piidEvents) return nullptr;
    if (::IsEqualGUID(iid, *pThis->m_piidEvents)) {
        // Retail returns &this + 0x328; its XPropConnPt object starts 8 bytes
        // before the header's m_xPropConnPt slot (the header models both
        // connection-point objects as 96-byte arrays). The named member is the
        // closest model.
        return reinterpret_cast<IConnectionPoint*>(&pThis->m_xPropConnPt);
    }
    return nullptr;
}
// COleControl::KeyDown(USHORT*) — retail (0x1e3a00):
//     if (KeyDown event-map entry present) {
//         nShiftState = GetShiftState();
//         FireEvent(DISPID_KEYDOWN, params, pnChar, nShiftState);
//         if (*pnChar) OnKeyDown(pnChar, nShiftState);   // vtable slot 0x348
//     }
// The event-map gate (a bit test on the message-map flags) and the OnKeyDown
// virtual are not modeled in OpenMFC; the fire is unconditional, matching the
// DoClick precedent in global_ole-5_impl.cpp.
// Symbol: ?KeyDown@COleControl@@IEAAXPEAG@Z
extern "C" void MS_ABI impl__KeyDown_COleControl__IEAAXPEAG_Z(
    COleControl* pThis, unsigned short* pnChar)
{
    if (!pThis || !pnChar) return;
    pThis->FireKeyDown(pnChar, GetShiftState());
    // TODO(clean-room): retail also forwards to the virtual OnKeyDown
    // (vtable slot 0x348) when *pnChar != 0; not modeled in the header.
}
// COleControl::KeyUp(USHORT*) — retail (0x1e3970): mirror image of KeyDown,
// fires DISPID_KEYUP and forwards to the virtual OnKeyUp (vtable slot 0x350).
// Symbol: ?KeyUp@COleControl@@IEAAXPEAG@Z
extern "C" void MS_ABI impl__KeyUp_COleControl__IEAAXPEAG_Z(
    COleControl* pThis, unsigned short* pnChar)
{
    if (!pThis || !pnChar) return;
    pThis->FireKeyUp(pnChar, GetShiftState());
    // TODO(clean-room): retail also forwards to the virtual OnKeyUp
    // (vtable slot 0x350) when *pnChar != 0; not modeled in the header.
}
// COleControl::OnChar(UINT, UINT, UINT) — retail (0x1e3b70):
//     ch = (USHORT)nChar;
//     if (IsDBCSLeadByte((BYTE)nChar) && event-map bit 3) {
//         if (PeekMessageW(&msg, m_hWnd, WM_CHAR, WM_CHAR, PM_NOYIELD))
//             ch = (nChar << 8) | LOWORD(msg.wParam);      // merge the pair
//     }
//     FireEvent(DISPID_KEYPRESS, params, &ch);
//     if (ch) OnKeyPress(&ch);                             // vtable slot 0x358
// Symbol: ?OnChar@COleControl@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnChar_COleControl__IEAAXIII_Z(
    COleControl* pThis, unsigned int nChar, unsigned int nRepCnt,
    unsigned int nFlags)
{
    if (!pThis) return;
    (void)nRepCnt;
    (void)nFlags;
    unsigned short ch = static_cast<unsigned short>(nChar);
    if (::IsDBCSLeadByte(static_cast<BYTE>(nChar)) && pThis->m_hWnd) {
        MSG msg;
        if (::PeekMessageW(&msg, pThis->m_hWnd, WM_CHAR, WM_CHAR, PM_NOYIELD)) {
            ch = static_cast<unsigned short>((nChar << 8) |
                                             (msg.wParam & 0xFFFF));
        }
    }
    pThis->FireKeyPress(&ch);
    // TODO(clean-room): retail also forwards to the virtual OnKeyPress
    // (vtable slot 0x358) when ch != 0; not modeled in the header.
}
// COleControl::OnClick(USHORT) — retail (0x1e43e0) fires DISPID_CLICK
// (event-map gate omitted, as for DoClick).
// Symbol: ?OnClick@COleControl@@UEAAXG@Z
extern "C" void MS_ABI impl__OnClick_COleControl__UEAAXG_Z(
    COleControl* pThis, unsigned short /*nButton*/)
{
    if (!pThis) return;
    pThis->FireClick();
}
// COleControl::OnButtonDown(USHORT, UINT, CPoint) — retail (0x1e4280):
//     if (nButton == MK_LBUTTON) SetFocus();   // windowless-aware
//     ButtonDown(nButton, nShiftState, point);
//     if (m_hWnd unchanged) <message-map notify>
// Symbol: ?OnButtonDown@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    if (nButton == 1 /* MK_LBUTTON */) {
        if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
            void* pSite = pThis->m_pInPlaceSite;
            if (pSite) {
                // IOleInPlaceSiteWindowless::SetFocus(TRUE) is slot 22.
                void** vft = *static_cast<void***>(pSite);
                auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[22]);
                fn(pSite, TRUE);
            }
        } else if (pThis->m_hWnd) {
            ::SetFocus(pThis->m_hWnd);
        }
    }
    impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    // TODO(clean-room): retail notifies the message-map machinery when the
    // window is unchanged; not modeled.
}
// COleControl::OnButtonDblClk(USHORT, UINT, CPoint) — retail (0x1e4370):
//     ButtonDblClk(nButton, nShiftState, point);
//     if (m_hWnd unchanged) <message-map notify>
// Symbol: ?OnButtonDblClk@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    // TODO(clean-room): retail notifies the message-map machinery when the
    // window is unchanged; not modeled.
}
// COleControl::OnButtonUp(USHORT, UINT, CPoint) — retail (0x1e42d0):
//     ButtonUp(nButton, nShiftState, point);
//     if (m_hWnd unchanged && (flags & 0x1c00) == 0x1400) {
//         // m_bInPlaceActive && !m_bUIActive && m_bPendingUIActivation
//         m_bPendingUIActivation = 0;
//         if (GetFocus() != m_hWnd && !IsChild(m_hWnd, GetFocus()))
//             <virtual SetFocus, vtable slot 0x508>;
//     }
// Symbol: ?OnButtonUp@COleControl@@IEAAXGIVCPoint@@@Z
extern "C" void MS_ABI impl__OnButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt)
{
    if (!pThis) return;
    impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
        pThis, nButton, nShiftState, pt);
    if (pThis->m_bInPlaceActive && !pThis->m_bUIActive &&
        pThis->m_bPendingUIActivation) {
        pThis->m_bPendingUIActivation = 0;
        HWND hFocus = ::GetFocus();
        if (hFocus != pThis->m_hWnd && !::IsChild(pThis->m_hWnd, hFocus)) {
            // TODO(clean-room): retail dispatches the windowless SetFocus
            // virtual (vtable slot 0x508); not modeled in the header.
        }
    }
}
// COleControl::OnCancelMode() — retail (0x1e43a0):
//     <message-map notify>
//     if (m_iButtonState || m_iDblClkState) {   // bits 4-9 of the 0x160 word
//         ReleaseCapture();                     // windowless-aware
//         m_iButtonState = 0;  m_iDblClkState = 0;
//     }
// Symbol: ?OnCancelMode@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): retail first forwards to the message-map machinery;
    // not modeled.
    if (pThis->m_iButtonState != 0 || pThis->m_iDblClkState != 0) {
        if (pThis->m_bInPlaceActive && pThis->m_bInPlaceSiteWndless) {
            void* pSite = pThis->m_pInPlaceSite;
            if (pSite) {
                // IOleInPlaceSiteWindowless::SetCapture(FALSE) is slot 20.
                void** vft = *static_cast<void***>(pSite);
                auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[20]);
                fn(pSite, FALSE);
            }
        } else {
            ::ReleaseCapture();
        }
        pThis->m_iButtonState = 0;
        pThis->m_iDblClkState = 0;
    }
}
// COleControl::OnAppearanceChanged() — retail (0x1edd40) is
// `xor edx,edx; lea r8d,0x1(rdx); jmp <InvalidateControl>`, i.e. exactly
// InvalidateControl(NULL, TRUE).
// Symbol: ?OnAppearanceChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnAppearanceChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnBackColorChanged() — retail alias of InvalidateControl
// (see file header).
// Symbol: ?OnBackColorChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnBackColorChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnBorderStyleChanged() — retail alias of InvalidateControl.
// Symbol: ?OnBorderStyleChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnBorderStyleChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnEnabledChanged() — retail alias of InvalidateControl.
// Symbol: ?OnEnabledChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnEnabledChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnForeColorChanged() — retail alias of InvalidateControl.
// Symbol: ?OnForeColorChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnForeColorChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnFontChanged() — retail (0x1ee280), when a window is
// attached, pushes the font holder's HFONT to the window with WM_SETFONT and
// remembers it in m_hFontPrev (resetting both when the holder has no font),
// then invalidates. The event-map gate and the PreCreateWindow check are not
// modeled.
// Symbol: ?OnFontChanged@COleControl@@UEAAXXZ
extern "C" void MS_ABI impl__OnFontChanged_COleControl__UEAAXXZ(
    COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_hWnd) {
        if (pThis->m_font.m_pFont != nullptr) {
            HFONT hFont = impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(
                &pThis->m_font);
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT,
                           reinterpret_cast<WPARAM>(hFont), TRUE);
            pThis->m_hFontPrev = hFont;
        } else {
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT, 0, FALSE);
            pThis->m_hFontPrev = nullptr;
        }
    }
    pThis->InvalidateControl(nullptr, TRUE);
}
// COleControl::OnDoVerb(long, MSG*, HWND, const RECT*) — retail (0x1e7350)
// tail-jumps to the OLE verb-map walker: the base control declares no verb
// map and reports FALSE for every verb.
// Symbol: ?OnDoVerb@COleControl@@UEAAHJPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__OnDoVerb_COleControl__UEAAHJPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(
    COleControl* pThis, long /*iVerb*/, MSG* /*lpmsg*/, HWND /*hwndParent*/,
    const RECT* /*lpRect*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. The verb-map walk and dispatch
    // are not modeled; the base class returns FALSE (verb not handled).
    return FALSE;
}
// COleControl::OnEnumVerbs(IEnumOLEVERB**) — retail (0x1e73d0) tail-jumps to
// the OLE verb-map enumerator builder. The base control has no verb map, so
// the out-pointer is set to NULL and FALSE is returned (no verbs to report).
// Symbol: ?OnEnumVerbs@COleControl@@UEAAHPEAPEAUIEnumOLEVERB@@@Z
extern "C" int MS_ABI impl__OnEnumVerbs_COleControl__UEAAHPEAPEAUIEnumOLEVERB___Z(
    COleControl* pThis, void** ppEnumOleVerb)
{
    if (!pThis || !ppEnumOleVerb) return FALSE;
    *ppEnumOleVerb = nullptr;
    // TODO(clean-room): transcribed partially. The verb-map enumeration is
    // not modeled; the base class reports no enumerator.
    return FALSE;
}
// COleControl::OnEdit(MSG*, HWND, const RECT*) — retail (0x1e1290):
//     CopyRect(&m_rcPos, lpRect);
//     return SUCCEEDED(<virtual edit activator, vtable slot 0x508>(1, lpmsg));
// The virtual is not modeled; the rect update is kept, the activation step
// reported as unhandled.
// Symbol: ?OnEdit@COleControl@@UEAAHPEAUtagMSG@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" int MS_ABI impl__OnEdit_COleControl__UEAAHPEAUtagMSG__PEAUHWND____PEBUtagRECT___Z(
    COleControl* pThis, MSG* /*lpmsg*/, HWND /*hwndParent*/, const RECT* lpRect)
{
    if (!pThis || !lpRect) return FALSE;
    ::CopyRect(pThis->m_rcPos, lpRect);
    // TODO(clean-room): retail dispatches the virtual in-place edit activator
    // (vtable slot 0x508) and returns SUCCEEDED(hr); not modeled.
    return FALSE;
}
// COleControl::OnDrawMetafile(CDC*, const CRect&) — retail (0x1e0600) simply
// forwards to OnDraw(pDC, rcBounds, rcBounds) through the vtable.
// Symbol: ?OnDrawMetafile@COleControl@@UEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawMetafile_COleControl__UEAAXPEAVCDC__AEBVCRect___Z(
    COleControl* pThis, CDC* pDC, const CRect& rcBounds)
{
    if (!pThis || !pDC) return;
    pThis->OnDraw(pDC, rcBounds, rcBounds);
}
// COleControl::OnEraseBkgnd(CDC*) — retail (0x1e1610):
//     if (PreCreateWindow()) return <message-map machinery>;
//     return 1;
// The base control's contract is "do not erase the background", reported as
// TRUE; the superclass message routing is not modeled.
// Symbol: ?OnEraseBkgnd@COleControl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_COleControl__IEAAHPEAVCDC___Z(
    COleControl* pThis, CDC* /*pDC*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially.
    return TRUE;
}
// COleControl::OnCreate(CREATESTRUCTW*) — retail (0x1e2140):
//     if (PreCreateWindow()) return <message-map machinery>;
//     return 0;
// The base control's success result is 0; the virtual PreCreateWindow gate
// and the message-map dispatch are not modeled.
// Symbol: ?OnCreate@COleControl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_COleControl__IEAAHPEAUtagCREATESTRUCTW___Z(
    COleControl* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    // TODO(clean-room): transcribed partially.
    return 0;
}
// COleControl::OnDestroy() — retail (0x1e2800):
//     if (m_hFontPrev) {
//         SendMessageW(m_hWnd, WM_SETFONT, 0, 0);
//         <release the old font through the CFontHolder>;
//         m_hFontPrev = NULL;
//     }
//     <CWnd::OnDestroy>
// The font-holder release and the base-class child destruction are not
// modeled; the WM_SETFONT reset and the handle drop are kept.
// Symbol: ?OnDestroy@COleControl@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_COleControl__IEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_hFontPrev) {
        if (pThis->m_hWnd) {
            ::SendMessageW(pThis->m_hWnd, WM_SETFONT, 0, 0);
        }
        pThis->m_hFontPrev = nullptr;
    }
    // TODO(clean-room): retail also releases the old font via the CFontHolder
    // and forwards to CWnd::OnDestroy; not modeled.
}
// COleControl::OnFinalRelease() — retail (0x1dfbe0):
//     if (m_bFinalReleaseCalled) return;
//     m_bFinalReleaseCalled = 1;
//     <in-place teardown>
//     if (m_hWnd) DestroyWindow();
//     <delete this>
// The delete is deliberately omitted (OpenMFC objects may be owned by the
// caller); the guard and window destruction are kept.
// Symbol: ?OnFinalRelease@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    if (pThis->m_bFinalReleaseCalled) return;
    pThis->m_bFinalReleaseCalled = 1;
    if (pThis->m_hWnd) {
        pThis->DestroyWindow();
    }
    // TODO(clean-room): retail releases the in-place state and deletes the
    // control; both are not modeled.
}
// COleControl::OnFrameClose() — retail (0x1e1010): hides, reparents and
// destroys the open frame window (m_pReflect, else the control itself), then
// clears m_bOpen/m_pWndOpenFrame, closes the OLE object and notifies the
// advise holder. The notification steps are not modeled.
// Symbol: ?OnFrameClose@COleControl@@MEAAXXZ
extern "C" void MS_ABI impl__OnFrameClose_COleControl__MEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    CWnd* pFrame = pThis->m_pReflect;
    if (pFrame == nullptr) {
        pFrame = static_cast<CWnd*>(pThis);
    }
    if (pFrame && pFrame->m_hWnd) {
        // Retail flags: SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|
        // SWP_HIDEWINDOW (0x97).
        ::SetWindowPos(pFrame->m_hWnd, nullptr, 0, 0, 0, 0, 0x97);
        ::SetParent(pFrame->m_hWnd, nullptr);
        ::DestroyWindow(pFrame->m_hWnd);
    }
    pThis->m_bOpen = 0;   // btrl $0xd on the 0x160 word
    pThis->m_pWndOpenFrame = nullptr;
    if (pThis->m_xOleObject) {
        // IOleObject::Close(OLECLOSE_SAVEIFDIRTY) is slot 6 of the object's
        // vtable.
        void* pObj = pThis->m_xOleObject;
        void** vft = *static_cast<void***>(pObj);
        auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, DWORD)>(vft[6]);
        fn(pObj, OLECLOSE_SAVEIFDIRTY);
    }
    // TODO(clean-room): retail also notifies the in-place active object and
    // the OLE advise holder; not modeled.
}
// COleControl::OnEnterIdle(UINT, CWnd*) — retail (0x1e2640):
//     if (nIdle != MSGF_DIALOGBOX) return;
//     if (m_nIDTracking == m_nIDLastMessage) return;
//     SendMessageW(m_hWnd, WM_ENTERIDLE, m_nIDTracking, 0);
// Symbol: ?OnEnterIdle@COleControl@@IEAAXIPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnEnterIdle_COleControl__IEAAXIPEAVCWnd___Z(
    COleControl* pThis, unsigned int nIdle, CWnd* /*pWnd*/)
{
    if (!pThis) return;
    if (nIdle != MSGF_DIALOGBOX) return;
    if (pThis->m_nIDTracking == pThis->m_nIDLastMessage) return;
    if (pThis->m_hWnd) {
        ::SendMessageW(pThis->m_hWnd, WM_ENTERIDLE,
                       static_cast<WPARAM>(pThis->m_nIDTracking), 0);
    }
}
// COleControl::OnActivateInPlace(int, MSG*) — retail (0x1e4c40) is the full
// in-place activation handshake. Only the early paths are reproducible:
// the m_bOpen fast path (activate the open frame window) and the null-site
// E_UNEXPECTED result; the windowless negotiation is not modeled.
// Symbol: ?OnActivateInPlace@COleControl@@MEAAJHPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnActivateInPlace_COleControl__MEAAJHPEAUtagMSG___Z(
    COleControl* pThis, int /*lVerb*/, MSG* /*lpmsg*/)
{
    if (!pThis) return 0x8000ffff;   // E_UNEXPECTED
    if (pThis->m_bOpen) {
        CWnd* pFrame = pThis->m_pWndOpenFrame;
        if (pFrame && pFrame->m_hWnd) {
            ::SetActiveWindow(pFrame->m_hWnd);
        }
        // TODO(clean-room): retail also routes the message through the
        // container's TranslateAccelerator; not modeled.
        return 0;   // S_OK
    }
    if (!pThis->m_pInPlaceSite && !pThis->m_pClientSite) {
        return 0x8000ffff;   // E_UNEXPECTED
    }
    // TODO(clean-room): transcribed partially. The site negotiation,
    // windowless activation and SetObjectRects handshake are not modeled;
    // report success without side effects.
    return 0;   // S_OK
}
// COleControl::OnEventAdvise(int) — retail (0x27d0) is a bare `ret $0x0`:
// the base control takes no action when a container connects/disconnects an
// event sink (derived controls use this to delay event work).
// Symbol: ?OnEventAdvise@COleControl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnEventAdvise_COleControl__UEAAXH_Z(
    COleControl* /*pThis*/, int /*fAdvise*/)
{
}
// COleControl::OnFreezeEvents(int) — retail (0x27d0) is a bare `ret $0x0`.
// Symbol: ?OnFreezeEvents@COleControl@@UEAAXH@Z
extern "C" void MS_ABI impl__OnFreezeEvents_COleControl__UEAAXH_Z(
    COleControl* /*pThis*/, int /*bFreeze*/)
{
}
// COleControl::Load(const wchar_t*, CDataPathProperty&) — retail (0x1ef410):
//     prop.SetControl(this);                 ; store this at prop+0x60
//     prop.Open(lpszPath, 0);                ; virtual, vtable slot 0xe0
// CDataPathProperty is not declared in OpenMFC, so the parameter is carried
// as void* and the two retail offsets are used directly.
// Symbol: ?Load@COleControl@@QEAAXPEB_WAEAVCDataPathProperty@@@Z
extern "C" void MS_ABI impl__Load_COleControl__QEAAXPEB_WAEAVCDataPathProperty___Z(
    COleControl* pThis, const wchar_t* lpszPath, void* pProp)
{
    if (!pThis || !pProp) return;
    // TODO(clean-room): CDataPathProperty is not declared in OpenMFC; the
    // 0x60 member slot and vtable slot 0xe0 come from the retail disassembly.
    *reinterpret_cast<void**>(static_cast<char*>(pProp) + 0x60) = pThis;
    void** vft = *static_cast<void***>(pProp);
    auto* fn = reinterpret_cast<void (STDMETHODCALLTYPE*)(void*, const wchar_t*, unsigned int)>(vft[0xe0 / 8]);
    fn(pProp, lpszPath, 0);
}
// COleControl::LoadState(IStream*) — retail (0x1e0b40) wraps the stream in a
// CArchive, invokes the virtual Serialize (vtable slot 0x10) and then clears
// m_bModified while raising m_bInitialized|m_bAutoMenuEnable. The CArchive
// construction is not modeled; report success without side effects.
// Symbol: ?LoadState@COleControl@@IEAAJPEAUIStream@@@Z
extern "C" long MS_ABI impl__LoadState_COleControl__IEAAJPEAUIStream___Z(
    COleControl* pThis, void* /*pStream*/)
{
    if (!pThis) return 0;   // S_OK
    // TODO(clean-room): transcribed partially.
    return 0;   // S_OK
}
// COleControl::LockInPlaceActive(int) — retail (0x1e1730):
//     if (!m_pControlSite) return FALSE;
//     return SUCCEEDED(m_pControlSite->LockInPlaceActive(bLock));
// IOleControlSite::LockInPlaceActive is slot 4 of the site vtable.
// Symbol: ?LockInPlaceActive@COleControl@@QEAAHH@Z
extern "C" int MS_ABI impl__LockInPlaceActive_COleControl__QEAAHH_Z(
    COleControl* pThis, int bLock)
{
    if (!pThis) return FALSE;
    void* pSite = pThis->m_pControlSite;
    if (!pSite) return FALSE;
    void** vft = *static_cast<void***>(pSite);
    auto* fn = reinterpret_cast<HRESULT (STDMETHODCALLTYPE*)(void*, BOOL)>(vft[4]);
    return SUCCEEDED(fn(pSite, bLock ? TRUE : FALSE)) ? TRUE : FALSE;
}
// COleControl::GetClientSite() — retail is a single load:
//     mov rax,[rcx+0x1d8] ; ret          (0x1d8 == 472 == m_pClientSite)
// Note it does NOT AddRef, matching the raw-pointer accessor contract.
// Symbol: ?GetClientSite@COleControl@@QEAAPEAUIOleClientSite@@XZ
extern "C" IOleClientSite* MS_ABI impl__GetClientSite_COleControl__QEAAPEAUIOleClientSite__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return pThis->m_pClientSite;
}
// COleControl::IsModified() — retail:
//     mov eax,[rcx+0x160] ; shr eax,2 ; and eax,1 ; ret
// i.e. it returns the m_bModified bit directly, not a normalized TRUE/FALSE
// from any other state.
// Symbol: ?IsModified@COleControl@@QEAAHXZ
extern "C" int MS_ABI impl__IsModified_COleControl__QEAAHXZ(COleControl* pThis)
{
    if (!pThis) return FALSE;
    return static_cast<int>(pThis->m_bModified);
}
// COleControl::GetActivationPolicy() — retail is literally `xor eax,eax ; ret`.
// The base control declares no activation policy; derived controls override to
// return POINTERINACTIVE_* flags.
// Symbol: ?GetActivationPolicy@COleControl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__GetActivationPolicy_COleControl__UEAAKXZ(
    COleControl* /*pThis*/)
{
    return 0;
}
// COleControl::GetRectInContainer(RECT*) — retail decodes to:
//     if (m_bInPlaceActive)              // test dword [rcx+0x160],0x400
//         CopyRect(lpRect, &m_rcPos);    // lea rdx,[rcx+0x120]
//     return m_bInPlaceActive;           // shr eax,0xa ; and eax,1
// The rect is left untouched when the control is not in-place active, and the
// return value is the same bit that gated the copy.
// Symbol: ?GetRectInContainer@COleControl@@QEAAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetRectInContainer_COleControl__QEAAHPEAUtagRECT___Z(
    COleControl* pThis, RECT* lpRect)
{
    if (!pThis) return FALSE;
    const int bInPlaceActive = static_cast<int>(pThis->m_bInPlaceActive);
    if (bInPlaceActive && lpRect) {
        ::CopyRect(lpRect, reinterpret_cast<const RECT*>(&pThis->m_rcPos));
    }
    return bInPlaceActive;
}
// COleControl::GetAmbientDispatchDriver() — retail lazily binds the driver to
// the container's IDispatch the first time it is asked:
//     lea rbx,[rcx+0x100]                 ; &m_ambientDispDriver
//     cmp qword ptr [rbx],0 ; jne done    ; already attached?
//     mov rcx,[rcx+0x1d8]                 ; m_pClientSite
//     ... QueryInterface(IID_IDispatch) and attach ...
// It always returns the address of the embedded driver, attached or not.
// Symbol: ?GetAmbientDispatchDriver@COleControl@@IEAAPEAVCOleDispatchDriver@@XZ
extern "C" COleDispatchDriver* MS_ABI
impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(COleControl* pThis)
{
    if (!pThis) return nullptr;
    COleDispatchDriver* pDriver = &pThis->m_ambientDispDriver;
    // m_lpDispatch is private; GetIDispatch(FALSE) reads the same slot without
    // taking a reference.
    if (pDriver->GetIDispatch(FALSE) == nullptr && pThis->m_pClientSite != nullptr) {
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(pThis->m_pClientSite->QueryInterface(IID_IDispatch,
                                                           reinterpret_cast<void**>(&pDisp)))
            && pDisp != nullptr) {
            // AttachDispatch takes ownership of the reference QueryInterface
            // handed back, matching retail (no extra AddRef at this site).
            pDriver->AttachDispatch(pDisp, TRUE);
        }
    }
    return pDriver;
}
// COleControl::EnableSimpleFrame() — retail is a single bit set:
//     bts DWORD PTR [rcx+0x160],0x10 ; ret
// It only raises the flag; it does not create the frame or notify the
// container. There is no matching disable entry point in retail either.
// Symbol: ?EnableSimpleFrame@COleControl@@QEAAXXZ
extern "C" void MS_ABI impl__EnableSimpleFrame_COleControl__QEAAXXZ(COleControl* pThis)
{
    if (!pThis) return;
    pThis->m_bSimpleFrame = 1;   // bit 16 of the 0x160 word
}
// COleControl::IsInvokeAllowed(DISPID) — retail:
//     mov eax,[rcx+0x160] ; shr eax,0x12 ; and eax,1 ; ret
// Bit 18 is returned directly and the DISPID argument is ignored entirely --
// the base class makes no per-member decision, it reports whether the control
// is in a state that permits automation at all. Derived classes override.
// Symbol: ?IsInvokeAllowed@COleControl@@MEAAHJ@Z
extern "C" int MS_ABI impl__IsInvokeAllowed_COleControl__MEAAHJ_Z(
    COleControl* pThis, long /*dispid*/)
{
    if (!pThis) return FALSE;
    return pThis->m_bInitialized ? TRUE : FALSE;   // bit 18 of the 0x160 word
}
// COleControl::GetOuterWindow() const — retail:
//     mov rax,[rcx+0x150] ; test rax,rax ; cmovne rcx,rax ; mov rax,rcx ; ret
// i.e. the reflected window if one is installed, otherwise the control itself.
// The cmov makes the fallback branchless but the semantics are a plain
// null-check; note it never returns null for a valid control.
// Symbol: ?GetOuterWindow@COleControl@@IEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetOuterWindow_COleControl__IEBAPEAVCWnd__XZ(
    const COleControl* pThis)
{
    if (!pThis) return nullptr;
    CWnd* pReflect = pThis->m_pReflect;
    return pReflect != nullptr ? pReflect
                               : const_cast<CWnd*>(static_cast<const CWnd*>(pThis));
}
// COleControl::InternalGetFont() — retail is `lea rax,[rcx+0x178] ; ret`:
// it hands back a reference to the embedded CFontHolder, not a copy and not a
// COM pointer, so callers mutate the control's own font state through it.
// Symbol: ?InternalGetFont@COleControl@@QEAAAEAVCFontHolder@@XZ
extern "C" CFontHolder* MS_ABI impl__InternalGetFont_COleControl__QEAAAEAVCFontHolder__XZ(
    COleControl* pThis)
{
    if (!pThis) return nullptr;
    return &pThis->m_font;
}
// COleControl::GetWindowlessDropTarget() — retail is `xor eax,eax ; ret`.
// The base control supplies no drop target; a windowless control that wants
// drag-and-drop overrides this to return one.
// Symbol: ?GetWindowlessDropTarget@COleControl@@UEAAPEAUIDropTarget@@XZ
extern "C" void* MS_ABI impl__GetWindowlessDropTarget_COleControl__UEAAPEAUIDropTarget__XZ(
    COleControl* /*pThis*/)
{
    return nullptr;
}
// COleControl::OnCreateAggregates() — retail is `mov eax,0x1 ; ret`.
// The base class aggregates nothing and reports success unconditionally;
// derived controls override to create their aggregates and may fail.
// Symbol: ?OnCreateAggregates@COleControl@@MEAAHXZ
extern "C" int MS_ABI impl__OnCreateAggregates_COleControl__MEAAHXZ(COleControl* /*pThis*/)
{
    return TRUE;
}
// COleControl::GetPropPageIDs(DWORD&) — retail:
//     mov DWORD PTR [rdx],0x0 ; xor eax,eax ; ret
// The base control publishes no property pages: it writes a zero count through
// the out-parameter and returns a null array. Writing the count matters --
// the generated stub left the caller's DWORD untouched, so a container could
// walk a null array using stack garbage as the length.
// Symbol: ?GetPropPageIDs@COleControl@@MEAAPEAU_GUID@@AEAK@Z
extern "C" GUID* MS_ABI impl__GetPropPageIDs_COleControl__MEAAPEAU_GUID__AEAK_Z(
    COleControl* /*pThis*/, DWORD* pcPages)
{
    if (pcPages) *pcPages = 0;
    return nullptr;
}
// Symbol: ??0CControlDataSource@COleControl@@QEAA@PEAV1@@Z
extern "C" void* MS_ABI impl___0CControlDataSource_COleControl__QEAA_PEAV1__Z(void* pThis, void* pContainer) {
    (void)pContainer;
    return pThis;
}
// Symbol: ?GetDataSource@COleControl@@IEAAPEAVCControlDataSource@1@XZ
extern "C" void* MS_ABI impl__GetDataSource_COleControl__IEAAPEAVCControlDataSource_1_XZ(
    void* p0, void* /*p1*/, void* /*p2*/, void* /*p3*/, void* /*p4*/, void* /*p5*/, void* /*p6*/,
    char /*p7*/, void* /*p8*/, void* /*p9*/, void* /*p10*/, void* /*p11*/, void* /*p12*/, void* /*p13*/,
    void* /*p14*/, void* /*p15*/, void* /*p16*/, char /*p17*/) {
    auto* control = static_cast<COleControl*>(p0);
    if (!control) return nullptr;

    if (!control->m_pDataSource) {
        return new COleControl::CControlDataSource(control);
    }

    return control->m_pDataSource;
}
// Symbol: ?OnRenderData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    FORMATETC request = MakeFormatEtc(0, p0Format);
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, request, false);
    if (!entry || !entry->hasMedium) return FALSE;

    return CopyStorageMedium(entry->medium, p1) ? TRUE : FALSE;
}
// Symbol: ?OnRenderFileData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAVCFile___Z(
    void* p0, FORMATETC* p0Format, CFile* p1) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium) return FALSE;

    const STGMEDIUM& medium = entry->medium;
    if (medium.tymed == TYMED_HGLOBAL && medium.hGlobal) {
        SIZE_T size = GlobalSize(medium.hGlobal);
        void* data = GlobalLock(medium.hGlobal);
        if (!data) return FALSE;
        p1->Write(data, static_cast<UINT>(size));
        GlobalUnlock(medium.hGlobal);
        return TRUE;
    }

    if (medium.tymed == TYMED_FILE && medium.lpszFileName) {
        CFile sourceFile(medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (sourceFile.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096] = {};
        UINT read = 0;
        while ((read = sourceFile.Read(buffer, sizeof(buffer))) > 0) {
            p1->Write(buffer, read);
        }
        return TRUE;
    }

    return FALSE;
}
// Symbol: ?OnRenderGlobalData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAPEAX_Z(
    void* p0, FORMATETC* p0Format, void** p0Medium) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    if (!p0Format) return FALSE;
    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, false);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL || !entry->medium.hGlobal) {
        *p0Medium = nullptr;
        return FALSE;
    }

    *p0Medium = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *p0Medium != nullptr;
}
// Symbol: ?OnSetData@CControlDataSource@COleControl@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_CControlDataSource_COleControl__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* p0, FORMATETC* p0Format, STGMEDIUM* p1Medium, int /*p2*/) {
    auto* source = static_cast<COleControl::CControlDataSource*>(p0);
    if (!source || !p0Format || !p1Medium) return FALSE;

    COleControl* control = GetControlFromDataSource(source);
    if (!control) return FALSE;

    OleControlDataSourceCacheEntry* entry = FindControlDataSourceEntry(control, *p0Format, true);
    if (!entry) return FALSE;

    ReleaseDataSourceEntryMedium(*entry);
    entry->format = MakeFormatEtc(0, p0Format);
    entry->hasMedium = CopyStorageMedium(*p1Medium, &entry->medium);
    return entry->hasMedium ? TRUE : FALSE;
}
COleControl::CControlDataSource::CControlDataSource(COleControl* pCtrl) {
    if (pCtrl) pCtrl->m_pDataSource = this;
}
COleControl::COleControl() {
    // Zero the whole retail member block, then set the members whose retail
    // initial values are not zero. Members live at fixed offsets now, so this
    // clears exactly the region a real client would inspect.
    std::memset(reinterpret_cast<char*>(this) + sizeof(CWnd), 0,
                sizeof(COleControl) - sizeof(CWnd));
    m_bAutoMenuEnable = 1;
    m_bAutoClip = 1;
    m_bCountOnAmbients = 1;
    m_sAppearance = 0;
    m_bEnabled = TRUE;
    m_lReadyState = 4;              // READYSTATE_COMPLETE
    m_clrBackColor = 0x80000005;    // COLOR_WINDOW  | 0x80000000
    m_clrForeColor = 0x80000008;    // COLOR_WINDOWTEXT | 0x80000000
    new (&m_strText) CString();
    new (&m_font) CFontHolder();
    new (&m_ambientDispDriver) COleDispatchDriver();
    GetOleControlState(this, true);
}
COleControl::~COleControl() {
    if (m_pDataSource) {
        auto* source = static_cast<COleControl::CControlDataSource*>(m_pDataSource);
        m_pDataSource = nullptr;
        delete source;
    }
    RemoveOleControlState(this);
}
BOOL COleControl::CreateControl(REFCLSID clsid, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!pParentWnd) return FALSE;

    COleControlContainer* container = pParentWnd->GetControlContainer();
    if (!container) {
        if (!pParentWnd->CreateControlContainer(&container) || !container) return FALSE;
    }

    COleControlSite* site = container->CreateSite(container);
    if (!site) return FALSE;
    site->m_pControl = this;

    if (!site->CreateControl(this, clsid, lpszWindowName, dwStyle, rect, nID, pPersist, bStorage, bstrLicKey)) {
        site->m_pControl = nullptr;
        container->DeleteSite(site);
        return FALSE;
    }

    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = container;
        state->mfcSite = site;
    }
    m_bInitialized = 1;
    return TRUE;
}
BOOL COleControl::CreateControl(const wchar_t* lpszProgID, const wchar_t* lpszWindowName,
                                 DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                                 UINT nID, CFile* pPersist, BOOL bStorage, BSTR bstrLicKey) {
    if (!lpszProgID) return FALSE;
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(lpszProgID, &clsid);
    if (FAILED(hr)) return FALSE;
    return CreateControl(clsid, lpszWindowName, dwStyle, rect, pParentWnd, nID, pPersist, bStorage, bstrLicKey);
}
BOOL COleControl::GetLicenseKey(DWORD dwReserved, BSTR* pbstrKey) {
    (void)dwReserved;
    if (pbstrKey) {
        *pbstrKey = nullptr;
    }

    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return FALSE;
    }

    if (pbstrKey) {
        *pbstrKey = AllocateLicenseBstr(state->licenseKey);
    }
    return TRUE;
}
BSTR COleControl::GetLicenseKey(REFCLSID clsid) {
    return AllocateLicenseBstr(FindFactoryLicense(clsid));
}
void COleControl::Serialize(CArchive& ar) {
    CWnd::Serialize(ar);
}
BOOL COleControl::DoPropExchange(CPropExchange* pPX) {
    return pPX != nullptr;
}
BOOL COleControl::GetAmbientProperty(DISPID dwDispid, VARTYPE vtProp, void* pvProp) {
    if (!pvProp) return FALSE;

    // OpenMFC's own CreateControl path records a COleControlSite in the side
    // table and that site answers ambients directly, so it stays the preferred
    // source. It is not the only way to be hosted, though: an ordinary COM
    // container calls SetClientSite and never touches the side table, and for
    // those controls every ambient lookup used to fail outright. Fall back to
    // the container's IDispatch, which is the path retail always takes.
    if (COleControlSite* pSite = MfcSiteOf(this))
        return pSite->GetAmbientProperty(dwDispid, vtProp, pvProp);

    COleDispatchDriver* pDriver =
        impl__GetAmbientDispatchDriver_COleControl__IEAAPEAVCOleDispatchDriver__XZ(this);
    if (pDriver == nullptr || pDriver->GetIDispatch(FALSE) == nullptr)
        return FALSE;

    // DISPATCH_PROPERTYGET == 2. A container that does not implement the
    // ambient reports failure through the driver rather than by throwing here.
    pDriver->InvokeHelper(dwDispid, 2, vtProp, pvProp, nullptr);
    return TRUE;
}
void COleControl::FireEvent(DISPID dispId, BYTE* pbParams, ...) {
    va_list args;
    va_start(args, pbParams);
    FireEventV(dispId, pbParams, args);
    va_end(args);
}
void COleControl::FireEventV(DISPID dispId, BYTE* pbParams, va_list argList) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->eventSinks.empty()) return;

    // Build DISPPARAMS from the VT-encoded parameter list
    int cParams = CountDispatchParams(pbParams);
    std::vector<VARIANTARG> params(static_cast<size_t>(cParams));
    va_list argCopy;
    // We need a copy of argList per each sink invocation
    // Build params once; they are read-only for each Invoke call
    va_copy(argCopy, argList);
    for (int i = 0; i < cParams; ++i) {
        MakeDispatchVariant(static_cast<VARTYPE>(pbParams[i]), &argCopy, &params[static_cast<size_t>(i)]);
    }
    va_end(argCopy);

    // Reverse for DISPPARAMS (COM convention: last arg first)
    std::vector<VARIANTARG> revParams(static_cast<size_t>(cParams));
    for (int i = 0; i < cParams; ++i) {
        revParams[static_cast<size_t>(i)] = params[static_cast<size_t>(cParams - i - 1)];
    }

    DISPPARAMS dp = {};
    dp.cArgs = static_cast<UINT>(cParams);
    dp.rgvarg = cParams ? revParams.data() : nullptr;

    // Fire to all registered sinks
    for (auto& sink : state->eventSinks) {
        if (!sink.sink) continue;
        IDispatch* pDisp = nullptr;
        if (SUCCEEDED(sink.sink->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pDisp))) && pDisp) {
            pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT,
                          DISPATCH_METHOD, &dp, nullptr, nullptr, nullptr);
            pDisp->Release();
        }
    }

    // Clean up variants (don't use VariantClear on the reversed view; clear originals)
    for (int i = 0; i < cParams; ++i) {
        params[static_cast<size_t>(i)].vt = VT_EMPTY;
        VariantClear(&params[static_cast<size_t>(i)]);
    }
}
BOOL COleControl::IsOptimizedDraw() const {
    return m_bOptimizedDraw;
}
void COleControl::SetInitialSize(int cx, int cy) {
    SetControlSize(cx, cy);
}
void COleControl::OnDraw(CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid) {
    (void)rcInvalid;
    if (!pDC || !pDC->GetSafeHdc()) return;

    RECT rc = *rcBounds;
    const COLORREF backColor = m_bEnabled ? AmbientBackColor() : RGB(240, 240, 240);
    HBRUSH hBrush = ::CreateSolidBrush(backColor);
    if (hBrush) {
        ::FillRect(pDC->GetSafeHdc(), &rc, hBrush);
        ::DeleteObject(hBrush);
    } else {
        ::FillRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    }
    ::FrameRect(pDC->GetSafeHdc(), &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));

    const wchar_t* text = m_strText.IsEmpty() ? L"ActiveX Control" : m_strText.GetString();
    int oldMode = ::SetBkMode(pDC->GetSafeHdc(), TRANSPARENT);
    ::SetTextColor(pDC->GetSafeHdc(), AmbientForeColor());
    ::DrawTextW(pDC->GetSafeHdc(), text, -1, &rc,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    ::SetBkMode(pDC->GetSafeHdc(), oldMode);
}
BOOL COleControl::VerifyUserLicense() { return TRUE; }
BOOL COleControl::VerifyLicenseKey(BSTR bstrKey) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state || state->licenseKey.IsEmpty()) {
        return TRUE;
    }
    CString key = bstrKey ? bstrKey : L"";
    return key == state->licenseKey;
}
BOOL COleControl::SetLicenseKey(const wchar_t* lpszLicenseKey) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return FALSE;
    state->licenseKey = lpszLicenseKey ? lpszLicenseKey : L"";
    return TRUE;
}
void COleControl::DoDataExchange(void* pDX) { (void)pDX; }
void COleControl::OnResetState() {
    OleControlState* state = GetOleControlState(this, true);
    if (state) state->text.Empty();
    m_strText.Empty();
    // The properties below now live in the ABI-visible members, so the reset
    // has to clear those rather than the side-table shadow a client cannot
    // see. Values match what the retail constructor establishes.
    m_clrBackColor = 0x80000005;   // COLOR_WINDOW     | 0x80000000
    m_clrForeColor = 0x80000008;   // COLOR_WINDOWTEXT | 0x80000000
    m_bEnabled     = TRUE;
    m_sAppearance  = 0;
    m_sBorderStyle = 0;
    m_lReadyState  = 4;            // READYSTATE_COMPLETE
    // The side table is not the ABI-visible copy: IsModified() reads the
    // m_bModified bit at this+0x160, so the reset has to clear that too or a
    // freshly reset control still reports itself dirty to a real client.
    m_bModified = 0;
    m_cxExtent = 0;
    m_cyExtent = 0;
}
DWORD COleControl::GetControlFlags() {
    return 0x00000002;   // clipPaintDC
}
BOOL COleControl::OnSetExtent(DVASPECT dwDrawAspect, const SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    m_cxExtent = size.cx;
    m_cyExtent = size.cy;
    return TRUE;
}
BOOL COleControl::OnGetExtent(DVASPECT dwDrawAspect, SIZE& size) {
    if (dwDrawAspect != DVASPECT_CONTENT) return FALSE;
    size.cx = m_cxExtent;
    size.cy = m_cyExtent;
    return TRUE;
}
BOOL COleControl::OnMapPropertyToPage(DISPID dispid, CLSID* pclsid, BOOL* pbPageOptional) {
    if (!pclsid) return FALSE;
    *pclsid = CLSID_NULL;
    if (pbPageOptional) *pbPageOptional = FALSE;

    IPerPropertyBrowsing* browsing = QueryOleControlInterface<IPerPropertyBrowsing>(this, IID_IPerPropertyBrowsing);
    if (!browsing) return FALSE;

    CLSID clsid = CLSID_NULL;
    HRESULT hr = browsing->MapPropertyToPage(dispid, &clsid);
    browsing->Release();
    if (FAILED(hr) || IsEqualCLSID(clsid, CLSID_NULL)) return FALSE;

    *pclsid = clsid;
    return TRUE;
}
COLORREF COleControl::AmbientBackColor() { COLORREF cr = RGB(255,255,255); GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientForeColor() { COLORREF cr = RGB(0,0,0); GetAmbientProperty(DISPID_AMBIENT_FORECOLOR, VT_COLOR, &cr); return cr; }
COLORREF COleControl::AmbientAppearance() { short appearance = 0; GetAmbientProperty(DISPID_AMBIENT_APPEARANCE, VT_I2, &appearance); return static_cast<COLORREF>(appearance); }
OLE_COLOR COleControl::AmbientBackColorOle() { return (OLE_COLOR)AmbientBackColor(); }
OLE_COLOR COleControl::AmbientForeColorOle() { return (OLE_COLOR)AmbientForeColor(); }
IFontDisp* COleControl::AmbientFont() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return (IFontDisp*)p; }
IDispatch* COleControl::AmbientFontDisp() { IDispatch* p = nullptr; GetAmbientProperty(DISPID_AMBIENT_FONT, VT_DISPATCH, &p); return p; }
short COleControl::AmbientTextAlign() {
    short align = 0;
    GetAmbientProperty(DISPID_AMBIENT_TEXTALIGN, VT_I2, &align);
    return align;
}
BOOL COleControl::AmbientUserMode() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_USERMODE, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientUIDead() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_UIDEAD, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowGrabHandles() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientShowHatching() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, VT_BOOL, &b); return b; }
CString COleControl::AmbientDisplayName() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_DISPLAYNAME, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
BOOL COleControl::AmbientDisplayAsDefault() { BOOL b = FALSE; GetAmbientProperty(DISPID_AMBIENT_DISPLAYASDEFAULT, VT_BOOL, &b); return b; }
BOOL COleControl::AmbientAutoClip() { return TRUE; }
BOOL COleControl::AmbientSupportsMnemonics() { BOOL b = TRUE; GetAmbientProperty(DISPID_AMBIENT_SUPPORTSMNEMONICS, VT_BOOL, &b); return b; }
CString COleControl::AmbientScaleUnits() {
    BSTR bstr = nullptr;
    if (!GetAmbientProperty(DISPID_AMBIENT_SCALEUNITS, VT_BSTR, &bstr) || !bstr) return CString();
    CString text(bstr);
    SysFreeString(bstr);
    return text;
}
unsigned long COleControl::AmbientLocaleID() { unsigned long locale = static_cast<unsigned long>(::GetUserDefaultLCID()); GetAmbientProperty(DISPID_AMBIENT_LOCALEID, VT_I4, &locale); return locale; }
void COleControl::FireClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_CLICK, noParams);
}
void COleControl::FireDblClick() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_DBLCLICK, noParams);
}
void COleControl::FireKeyDown(USHORT* pnChar, short nShiftState) {
    // VT_PI2 (pointer to I2) = VT_BYREF|VT_I2 = 0x4002; MFC uses 0x4002 for USHORT*
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYDOWN, params, pnChar, nShiftState);
}
void COleControl::FireKeyPress(USHORT* pnChar) {
    BYTE params[2] = { static_cast<BYTE>(VT_BYREF | VT_I2), 0 };
    FireEvent(DISPID_KEYPRESS, params, pnChar);
}
void COleControl::FireKeyUp(USHORT* pnChar, short nShiftState) {
    BYTE params[3] = { static_cast<BYTE>(VT_BYREF | VT_I2), VT_I2, 0 };
    FireEvent(DISPID_KEYUP, params, pnChar, nShiftState);
}
void COleControl::FireMouseDown(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEDOWN, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseMove(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEMOVE, params, nButton, nShiftState, x, y);
}
void COleControl::FireMouseUp(short nButton, short nShiftState, long x, long y) {
    BYTE params[5] = { VT_I2, VT_I2, VT_I4, VT_I4, 0 };
    FireEvent(DISPID_MOUSEUP, params, nButton, nShiftState, x, y);
}
void COleControl::FireReadyStateChange() {
    BYTE noParams[1] = { 0 };
    FireEvent(DISPID_READYSTATECHANGE, noParams);
}
COLORREF COleControl::GetBackColor() const {
    return static_cast<COLORREF>(m_clrBackColor);
}
void COleControl::SetBackColor(COLORREF clr) {
    if (m_clrBackColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrBackColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
COLORREF COleControl::GetForeColor() const {
    return static_cast<COLORREF>(m_clrForeColor);
}
void COleControl::SetForeColor(COLORREF clr) {
    if (m_clrForeColor == static_cast<OLE_COLOR>(clr)) return;
    m_clrForeColor = static_cast<OLE_COLOR>(clr);
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
BOOL COleControl::GetEnabled() const {
    return m_bEnabled;
}
void COleControl::SetEnabled(BOOL bEnabled) {
    if (m_bEnabled == bEnabled) return;
    m_bEnabled = bEnabled;
    if (m_hWnd) ::EnableWindow(m_hWnd, bEnabled);
    SetModifiedFlag(TRUE);
    // A windowless control never sees ::EnableWindow, so this is its only
    // trigger to repaint in the grayed state.
    InvalidateControl();
}
void COleControl::SetFont(LPFONTDISP pFontDisp) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font, pFontDisp);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
void COleControl::SetFont(CFont* pFont) {
    impl__SetFont_CFontHolder__QEAAXPEAUIFont___Z(&m_font,
                                                  pFont ? pFont->GetSafeHandle() : nullptr);
    SetModifiedFlag(TRUE);
    InvalidateControl();

    if (m_hWnd) {
        ::SendMessageW(m_hWnd, WM_SETFONT,
                       reinterpret_cast<WPARAM>(impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(&m_font)),
                       TRUE);
    }
}
unsigned int COleControl::GetHwnd() {
    if (!m_bInPlaceActive && !m_bOpen) return 0;
    return (unsigned int)(uintptr_t)m_hWnd;
}
void COleControl::SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
OLE_COLOR COleControl::GetBackColorOle() const { return (OLE_COLOR)GetBackColor(); }
OLE_COLOR COleControl::GetForeColorOle() const { return (OLE_COLOR)GetForeColor(); }
void COleControl::SetBackColorOle(OLE_COLOR clr) { SetBackColor((COLORREF)clr); }
void COleControl::SetForeColorOle(OLE_COLOR clr) { SetForeColor((COLORREF)clr); }
short COleControl::GetAppearance() const {
    return m_sAppearance;
}
void COleControl::SetAppearance(short nAppearance) {
    if (m_sAppearance == nAppearance) return;
    m_sAppearance = nAppearance;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
short COleControl::GetBorderStyle() const {
    return m_sBorderStyle;
}
void COleControl::SetBorderStyle(short nBorderStyle) {
    if (m_sBorderStyle == nBorderStyle) return;
    m_sBorderStyle = nBorderStyle;
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
wchar_t* COleControl::GetText() {
    return const_cast<wchar_t*>(m_strText.GetString());
}
const CString& COleControl::InternalGetText() { return m_strText; }
void COleControl::SetText(const wchar_t* lpszText) {
    CString newText = lpszText ? lpszText : L"";
    if (m_strText == newText) return;
    m_strText = newText;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->text = newText;
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
void COleControl::GetText(CString& strText) const { strText = m_strText; }
long COleControl::GetReadyState() const {
    return m_lReadyState;
}
BOOL COleControl::IsSubclassedControl() {
    return m_hWnd != nullptr && MfcSiteOf(this) == nullptr;
}
void COleControl::SetModifiedFlag(BOOL bModified) {
    m_bModified = bModified;
}
BOOL COleControl::GetModifiedFlag() const {
    return m_bModified;
}
ULONG COleControl::InternalAddRef() { return 1; }
ULONG COleControl::InternalRelease() { return 1; }
ULONG COleControl::InternalQueryInterface(REFIID riid, void** ppv) {
    if (!ppv) return E_POINTER;
    *ppv = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        return MfcSiteOf(this)->m_lpObject->QueryInterface(riid, ppv);
    }
    return E_NOINTERFACE;
}
void COleControl::GetControlSize(int* pCX, int* pCY) {
    if (pCX) *pCX = m_cxExtent;
    if (pCY) *pCY = m_cyExtent;
}
BOOL COleControl::SetControlSize(int cx, int cy) {
    SIZE size = { cx, cy };
    if (OnSetExtent(DVASPECT_CONTENT, size)) {
        SetModifiedFlag(TRUE);
        return TRUE;
    }
    return FALSE;
}
void COleControl::OnSetClientSite() {
    COleControlSite* site = MfcSiteOf(this);
    m_bInitialized = (site != nullptr) ? 1u : 0u;
    if (OleControlState* state = GetOleControlState(this, true)) {
        state->container = site ? site->GetContainer() : nullptr;
    }
}
void COleControl::OnGetControlInfo(LPCONTROLINFO pControlInfo) {
    if (!pControlInfo) return;
    memset(pControlInfo, 0, sizeof(*pControlInfo));
    pControlInfo->cb = sizeof(*pControlInfo);

    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (control) {
        CONTROLINFO info = {};
        info.cb = sizeof(info);
        if (SUCCEEDED(control->GetControlInfo(&info))) {
            *pControlInfo = info;
        }
        control->Release();
    }
}
BOOL COleControl::OnMnemonic(LPMSG pMsg) {
    if (!pMsg) return FALSE;
    IOleControl* control = QueryOleControlInterface<IOleControl>(this, IID_IOleControl);
    if (!control) return FALSE;
    HRESULT hr = control->OnMnemonic(pMsg);
    control->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnAmbientPropertyChange(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    if (dispid == DISPID_AMBIENT_BACKCOLOR || dispid == DISPID_UNKNOWN) {
        state->backColor = AmbientBackColor();
        InvalidateControl();
    }
    if (dispid == DISPID_AMBIENT_FORECOLOR || dispid == DISPID_UNKNOWN) {
        state->foreColor = AmbientForeColor();
        InvalidateControl();
    }
}
void COleControl::BoundPropertyChanged(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (state) {
        for (auto* sink : state->propSinks) {
            if (sink) sink->OnChanged(dispid);
        }
    }
    SetModifiedFlag(TRUE);
    InvalidateControl();
}
BOOL COleControl::BoundPropertyRequestEdit(DISPID dispid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return TRUE;
    for (auto* sink : state->propSinks) {
        if (sink && FAILED(sink->OnRequestEdit(dispid))) return FALSE;
    }
    return TRUE;
}
void COleControl::InvalidateControl(LPCRECT lpRect, BOOL bErase) {
    if (m_hWnd) ::InvalidateRect(m_hWnd, lpRect, bErase);
}
int COleControl::OnProperties(MSG* pMsg, HWND hWnd, const RECT* lpRect) {
    (void)pMsg; (void)lpRect;
    if (COleControlSite* site = MfcSiteOf(this)) {
        site->ShowPropertyFrame();
        return TRUE;
    }
    HWND oldWnd = m_hWnd;
    if (!m_hWnd && hWnd) m_hWnd = hWnd;
    int count = GetPropertyPageCount();
    if (count > 0) ShowPropertyPages();
    if (!oldWnd && hWnd) m_hWnd = oldWnd;
    return count > 0 ? TRUE : FALSE;
}
void COleControl::ShowPropertyPages() {
    if (!m_hWnd) return;
    ISpecifyPropertyPages* pSPP = nullptr;
    if (FAILED(InternalQueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pSPP))) || !pSPP)
        return;
    CAUUID pages = {};
    if (SUCCEEDED(pSPP->GetPages(&pages)) && pages.cElems > 0) {
        IUnknown* pUnk = nullptr;
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
        OleCreatePropertyFrame(m_hWnd, 0, 0, nullptr,
                               pUnk ? 1u : 0u, pUnk ? &pUnk : nullptr,
                               pages.cElems, pages.pElems,
                               LOCALE_USER_DEFAULT, 0, nullptr);
        if (pUnk) pUnk->Release();
        CoTaskMemFree(pages.pElems);
    }
    pSPP->Release();
}
int COleControl::GetPropertyPageCount() const {
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        const_cast<COleControl*>(this)->InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return 0;

    ISpecifyPropertyPages* pages = nullptr;
    HRESULT hr = unknown->QueryInterface(IID_ISpecifyPropertyPages, reinterpret_cast<void**>(&pages));
    unknown->Release();
    if (FAILED(hr) || !pages) return 0;
    CAUUID cauuid = {};
    int count = 0;
    if (SUCCEEDED(pages->GetPages(&cauuid))) {
        count = static_cast<int>(cauuid.cElems);
        CoTaskMemFree(cauuid.pElems);
    }
    pages->Release();
    return count;
}
BOOL COleControl::IsPropertyPage(LPUNKNOWN lpUnk) {
    if (!lpUnk) return FALSE;
    IPropertyPage* page = nullptr;
    HRESULT hr = lpUnk->QueryInterface(IID_IPropertyPage, reinterpret_cast<void**>(&page));
    if (page) page->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
BOOL COleControl::CanCreateConnectionPoints() { return TRUE; }
void COleControl::EnableConnectionPoints() {
    OleControlState* state = GetOleControlState(this, true);
    if (!state) return;
    IConnectionPointContainer* container = nullptr;
    IUnknown* unknown = nullptr;
    if (MfcSiteOf(this) && MfcSiteOf(this)->m_lpObject) {
        unknown = MfcSiteOf(this)->m_lpObject;
        unknown->AddRef();
    } else {
        InternalQueryInterface(IID_IUnknown, reinterpret_cast<void**>(&unknown));
    }
    if (!unknown) return;
    HRESULT hr = unknown->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(&container));
    unknown->Release();
    if (FAILED(hr) || !container) return;

    IEnumConnectionPoints* enumPoints = nullptr;
    if (SUCCEEDED(container->EnumConnectionPoints(&enumPoints)) && enumPoints) {
        IConnectionPoint* point = nullptr;
        ULONG fetched = 0;
        while (enumPoints->Next(1, &point, &fetched) == S_OK && fetched == 1) {
            IID iid = IID_NULL;
            if (SUCCEEDED(point->GetConnectionInterface(&iid))) {
                bool exists = std::any_of(state->enabledConnectionPoints.begin(), state->enabledConnectionPoints.end(),
                    [&iid](const IID& value) { return IsEqualIID(value, iid); });
                if (!exists) state->enabledConnectionPoints.push_back(iid);
            }
            point->Release();
        }
        enumPoints->Release();
    }
    container->Release();
}
BOOL COleControl::IsConnectionPointEnabled(REFIID riid) {
    OleControlState* state = GetOleControlState(this, false);
    if (!state) return FALSE;
    for (const IID& iid : state->enabledConnectionPoints) {
        if (IsEqualIID(iid, riid)) return TRUE;
    }
    for (auto& sink : state->eventSinks) {
        if (IsEqualIID(sink.iid, riid)) return TRUE;
    }
    return FALSE;
}
void COleControl::FirePropChanged(DISPID dispid) {
    BoundPropertyChanged(dispid);
}
BOOL COleControl::PreTranslateMessage(MSG* pMsg) {
    if (!pMsg) return FALSE;
    IOleInPlaceActiveObject* activeObject = QueryOleControlInterface<IOleInPlaceActiveObject>(this, IID_IOleInPlaceActiveObject);
    if (!activeObject) return FALSE;
    HRESULT hr = activeObject->TranslateAccelerator(pMsg);
    activeObject->Release();
    return hr == S_OK ? TRUE : FALSE;
}
LONG COleControl::OnPosRectChange(LPCRECT lprcPosRect) {
    if (!lprcPosRect) return E_POINTER;
    RECT clipRect = *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state && state->hasObjectRects) {
        clipRect = state->clipRect;
    }
    return OnSetObjectRects(lprcPosRect, &clipRect) ? S_OK : E_FAIL;
}
BOOL COleControl::OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect) {
    if (!lprcPosRect) return FALSE;
    RECT clipRect = lprcClipRect ? *lprcClipRect : *lprcPosRect;
    OleControlState* state = GetOleControlState(this, true);
    if (state) {
        state->posRect = *lprcPosRect;
        state->clipRect = clipRect;
        state->hasObjectRects = TRUE;
    }
    COleControlSite* ipSite = MfcSiteOf(this);
    if (ipSite && ipSite->m_lpInPlaceObject &&
        SUCCEEDED(ipSite->m_lpInPlaceObject->SetObjectRects(lprcPosRect, &clipRect))) {
        return TRUE;
    }
    IOleInPlaceObject* inPlace = QueryOleControlInterface<IOleInPlaceObject>(this, IID_IOleInPlaceObject);
    if (!inPlace) return FALSE;
    HRESULT hr = inPlace->SetObjectRects(lprcPosRect, &clipRect);
    inPlace->Release();
    return SUCCEEDED(hr) ? TRUE : FALSE;
}
void COleControl::OnClose(DWORD dwSaveOption) {
    m_bOpen = FALSE;
    m_bInPlaceActive = FALSE;
    if (dwSaveOption == OLECLOSE_NOSAVE) {
        SetModifiedFlag(FALSE);
    }
}
CWnd* COleControl::SetCapture() {
    HWND previous = m_hWnd ? ::SetCapture(m_hWnd) : nullptr;
    return previous ? CWnd::FromHandle(previous) : nullptr;
}
BOOL COleControl::ReleaseCapture() { return ::ReleaseCapture(); }
void COleControl::BringWindowToTop() { if(m_hWnd) ::BringWindowToTop(m_hWnd); }
void COleControl::MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { if(m_hWnd) ::MoveWindow(m_hWnd, X, Y, nWidth, nHeight, bRepaint); }
void COleControl::MoveWindow(LPCRECT lpRect, BOOL bRepaint) { if(m_hWnd && lpRect) ::MoveWindow(m_hWnd, lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint); }
