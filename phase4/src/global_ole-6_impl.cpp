// COleControl methods, wave 6 (ole-6). Every function here was decoded from
// the retail mfc140u export (ordinal -> RVA -> objdump, see /tmp/ole6_dis/)
// rather than inferred, and the member offsets used are exactly the ones
// transcribed into include/openmfc/afxole.h.
//
// The retail aliases every stock-property change notification
// (OnAppearanceChanged, OnBackColorChanged, OnBorderStyleChanged,
// OnEnabledChanged, OnForeColorChanged) to the same body:
//     xor edx,edx; lea r8d,0x1(rdx); jmp <InvalidateControl>
// i.e. they are all `InvalidateControl(NULL, TRUE)` and fire no per-property
// event on their own (the property setters do that). They are therefore
// spelled here as direct calls to the OpenMFC C++ member.
//
// OnEventAdvise and OnFreezeEvents export the same bare `ret $0x0` stub in
// retail (RVA 0x27d0): the base control takes no action when a container
// connects an event sink or freezes events.
//
// OnDoVerb and OnEnumVerbs tail-jump into the OLE verb-map machinery; a base
// COleControl declares no verb map, so both report FALSE / a null enumerator.
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

#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout anchors used by the code below (offsets come from afxole.h comments
// and are asserted there too; these just catch accidental header regressions).
// COleControl is not standard-layout (virtuals), so GCC flags offsetof here;
// the sibling global_ole-5_impl.cpp relies on the same asserts. The benign
// diagnostic is suppressed for this block only.
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
static_assert(offsetof(COleControl, m_pWndOpenFrame) == 280, "m_pWndOpenFrame @280");
static_assert(offsetof(COleControl, m_pReflect) == 336, "m_pReflect @336");
static_assert(offsetof(COleControl, m_nIDTracking) == 344, "m_nIDTracking @344");
static_assert(offsetof(COleControl, m_nIDLastMessage) == 348, "m_nIDLastMessage @348");
static_assert(offsetof(COleControl, m_hFontPrev) == 400, "m_hFontPrev @400");
static_assert(offsetof(COleControl, m_pClientSite) == 472, "m_pClientSite @472");
static_assert(offsetof(COleControl, m_pInPlaceSite) == 480, "m_pInPlaceSite @480");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_xOleObject) == 616, "m_xOleObject @616");
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" void MS_ABI impl__ButtonDown_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" void MS_ABI impl__ButtonDblClk_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" void MS_ABI impl__ButtonUp_COleControl__IEAAXGIVCPoint___Z(
    COleControl* pThis, unsigned short nButton, unsigned int nShiftState,
    unsigned long long pt);
extern "C" HFONT MS_ABI impl__GetFontHandle_CFontHolder__QEAAPEAUHFONT____XZ(
    void* self);

// Retail computes the keyboard shift state with three GetKeyState calls
// (VK_SHIFT/VK_CONTROL/VK_MENU) folded into MK_SHIFT|MK_CONTROL|MK_ALT.
static inline short GetShiftState()
{
    short nShift = 0;
    if (::GetKeyState(VK_SHIFT) < 0)   nShift |= 1;   // MK_SHIFT
    if (::GetKeyState(VK_CONTROL) < 0) nShift |= 2;   // MK_CONTROL
    if (::GetKeyState(VK_MENU) < 0)    nShift |= 4;   // MK_ALT
    return nShift;
}

//=============================================================================
// Keyboard / mouse events
//=============================================================================

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

//=============================================================================
// Stock-property change notifications
//=============================================================================

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

//=============================================================================
// OLE verbs
//=============================================================================

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

//=============================================================================
// Drawing
//=============================================================================

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

//=============================================================================
// Window lifecycle
//=============================================================================

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

//=============================================================================
// Event advise / freeze
//=============================================================================

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

//=============================================================================
// Persistence
//=============================================================================

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
