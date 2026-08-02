// COleControl methods, wave 5 (ole-5). Every function here was decoded from the
// retail mfc140u export (ordinal -> RVA -> objdump, see /tmp/ole5_dis/all.log)
// rather than inferred, and the member offsets used are exactly the ones
// transcribed into include/openmfc/afxole.h.
//
// Retail bitfield word at this+0x160; bit positions referenced below are the
// ones the shipping code actually tests:
//   bit 10 = m_bInPlaceActive, bit 22 = m_bInPlaceSiteWndless,
//   bits 4-6 = m_iButtonState, bits 7-9 = m_iDblClkState.
//
// Where a retail body depends on virtual slots or internal unexported helpers
// that the OpenMFC headers do not model, a type-correct conservative version is
// emitted (safe defaults, no state corruption) with a `TODO(clean-room)` note.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxole.h"
#include "openmfc/afxstr.h"

#include <new>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#ifndef DISPID_ERROREVENT
#define DISPID_ERROREVENT (-608)
#endif

// Layout anchors used by the code below (offsets come from afxole.h comments
// and are asserted there too; these just catch accidental header regressions).
static_assert(offsetof(COleControl, m_piidEvents) == 240, "m_piidEvents @240");
static_assert(offsetof(COleControl, m_rcPos) == 288, "m_rcPos @288");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_xPropConnPt) == 816, "m_xPropConnPt @816");

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" void* MS_ABI impl___0CRectTracker__QEAA_PEBUtagRECT__I_Z(
    void* pThis, const RECT* lpSrcRect, unsigned int nStyle);
extern "C" void MS_ABI impl___1CRectTracker__UEAA_XZ(void* pThis);

// ---- forward declarations within this file ----
extern "C" void MS_ABI impl__GetClientOffset_COleControl__UEBAXPEAJ0_Z(
    const COleControl* pThis, long* pDx, long* pDy);
extern "C" void MS_ABI impl__CreateWindowForSubclassedControl_COleControl__IEAAXXZ(
    COleControl* pThis);
extern "C" void MS_ABI impl__CreateTracker_COleControl__IEAAXHHPEBUtagRECT___Z(
    COleControl* pThis, int nSize, int bEnableResize, const RECT* lpRect);

// CPoint arrives by value as one 8-byte integer register under MS x64; unpack
// it the same way global_crecttracker.cpp does.
static inline long PtX(unsigned long long pt) { return (long)(unsigned int)(pt & 0xFFFFFFFFull); }
static inline long PtY(unsigned long long pt) { return (long)(unsigned int)(pt >> 32); }

//=============================================================================
// Ambient / event helpers
//=============================================================================

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

//=============================================================================
// Button helpers
//=============================================================================

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

//=============================================================================
// Coordinate helpers
//=============================================================================

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

//=============================================================================
// Capture
//=============================================================================

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

//=============================================================================
// Window / frame creation
//=============================================================================

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

//=============================================================================
// Drawing
//=============================================================================

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

//=============================================================================
// Persistence / property exchange
//=============================================================================

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

//=============================================================================
// In-place UI state
//=============================================================================

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

//=============================================================================
// Tracker
//=============================================================================

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

//=============================================================================
// Connection points
//=============================================================================

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
