// CMFCAutoHideBar — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfcautohidebar.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// OpenMFC: CMFCAutoHideBar exports. This file contains a mix of direct
// transcriptions, side-table approximations, and explicit TODO stubs; it is not
// a complete implementation of the retail class.
//
// The OpenMFC declaration in include/openmfc/afxmfc.h models CMFCAutoHideBar as
// CPane plus 32 bytes of padding (0x418 bytes total). Retail allocates 0x458
// bytes and uses private state beyond that declaration. Its auto-hide-button
// list starts at +0x3f8 (list traversal uses +0x400); other retail state
// includes button pointers at +0x430/+0x438, a timer ID at +0x448, flags at
// +0x440/+0x450, and CPane::m_bActiveInGroup at +0x1b4. Most of that private
// state is not modeled here; some API approximations use the side tables in
// CbarcoreSupport.h, while operations requiring retail button objects remain
// stubs.
//
// Export thunks use `extern "C"` and `MS_ABI`. For struct-returning methods,
// the retail ABI places `this` in RCX and the hidden result pointer in RDX;
// definitions below preserve that order. Null handling is per-function rather
// than a universal property.
//
// The exact `// Symbol:` markers below make AUTO_EXCLUDES omit the generated
// weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstdint>
#include <cstring>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext);
extern "C" void MS_ABI impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(
    CWnd* pThis, unsigned int nFlags, CPoint point);
extern "C" int MS_ABI impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(
    CWnd* pWnd);
// AfxThrowInvalidArgException is only exported as an extern-C MS_ABI impl_
// function (mfc_exceptions.cpp); the C++ symbol the header declares has no
// definition, so the impl_ form is called directly (same ABI result).
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

//=============================================================================
// CMFCAutoHideBar - the OpenMFC class layout models CPane and a short padding
// tail; retail-only button/list state is intentionally not accessed directly.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CreateObject
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CalcFixedLayout
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CleanUpAutoHideButtons
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::Create
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::DoPaint
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnCreate
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnLButtonDown
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnMouseLeave
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnMouseMove
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnNcDestroy
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnTimer
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::RemoveAutoHideWindow
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::SetActiveInGroup
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::ShowAutoHideWindow
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::StretchPane
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::UnSetAutoHideMode
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::UpdateVisibleState
//-----------------------------------------------------------------------------
// Symbol: ??0CMFCAutoHideBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCAutoHideBar__QEAA_XZ(void* pThis) {
    if (!pThis) return nullptr;
    auto* pBar = new (pThis) CMFCAutoHideBar();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideBarWindows[pThis] = {};
    return pBar;
}
// Symbol: ??1CMFCAutoHideBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCAutoHideBar__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    {
        std::lock_guard<std::mutex> lock(g_wave2Mutex);
        auto it = g_autoHideBarWindows.find(pThis);
        if (it != g_autoHideBarWindows.end()) {
            for (void* pane : it->second.panes) {
                auto buttonIt = g_paneToAutoHideButton.find(pane);
                if (buttonIt != g_paneToAutoHideButton.end()) {
                    g_autoHideButtons.erase(buttonIt->second);
                    g_paneToAutoHideButton.erase(buttonIt);
                }
                g_autoHideButtonStorage.erase(pane);
            }
            g_autoHideBarWindows.erase(it);
        }
    }
    // Qualified call: a plain ->~CMFCAutoHideBar() dispatches virtually and would
    // re-enter a client-derived destructor.
    static_cast<CMFCAutoHideBar*>(pThis)->CMFCAutoHideBar::~CMFCAutoHideBar();
}
// OpenMFC side-table approximation: alignment is not retained and the returned
// token is not a retail CMFCAutoHideButton object.
// Symbol: ?AddAutoHideWindow@CMFCAutoHideBar@@QEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@K@Z
extern "C" void* MS_ABI impl__AddAutoHideWindow_CMFCAutoHideBar__QEAAPEAVCMFCAutoHideButton__PEAVCDockablePane__K_Z(
    void* pThis, void* pPane, unsigned long dwAlignment) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    (void)dwAlignment;
    AddUniquePane(g_autoHideBarWindows[pThis], pPane);
    if (pPane && g_paneToAutoHideButton.find(pPane) == g_paneToAutoHideButton.end()) {
        auto storage = std::make_unique<int>(0);
        void* buttonPtr = storage.get();
        g_autoHideButtonStorage[pPane] = std::move(storage);
        g_paneToAutoHideButton[pPane] = buttonPtr;
        g_autoHideButtons[buttonPtr] = { FALSE, FALSE };
    }
    return pPane ? g_paneToAutoHideButton[pPane] : nullptr;
}
// OpenMFC approximation: this lookup is global by pane and does not verify that
// the button belongs to pThis.
// Symbol: ?ButtonFromAutoHideWindow@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@@Z
extern "C" void* MS_ABI impl__ButtonFromAutoHideWindow_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__PEAVCDockablePane___Z(
    void*, void* pPane) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    auto it = g_paneToAutoHideButton.find(pPane);
    return it == g_paneToAutoHideButton.end() ? nullptr : it->second;
}
// OpenMFC approximation: ignores the point and returns the first registered
// pane's token; retail tests button rectangles (mfc140 RVA 0x9250, ANSI twin).
// Symbol: ?ButtonFromPoint@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@VCPoint@@@Z
extern "C" void* MS_ABI impl__ButtonFromPoint_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__VCPoint___Z(void* pThis, CPoint) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    void* pane = FirstPane(g_autoHideBarWindows[pThis]);
    auto it = g_paneToAutoHideButton.find(pane);
    return it == g_paneToAutoHideButton.end() ? nullptr : it->second;
}
// OpenMFC side-table approximation: returns the first registered pane.
// Symbol: ?GetFirstAHWindow@CMFCAutoHideBar@@QEAAPEAVCDockablePane@@XZ
extern "C" void* MS_ABI impl__GetFirstAHWindow_CMFCAutoHideBar__QEAAPEAVCDockablePane__XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return FirstPane(g_autoHideBarWindows[pThis]);
}
// OpenMFC approximation counts registered panes, not visible buttons. Retail
// counts buttons whose visibility virtual returns true (mfc140u RVA 0x8e90).
// Symbol: ?GetVisibleCount@CMFCAutoHideBar@@QEAAHXZ
extern "C" int MS_ABI impl__GetVisibleCount_CMFCAutoHideBar__QEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    return static_cast<int>(g_autoHideBarWindows[pThis].panes.size());
}
// Retail factory entry RVA 0x8950 (mfc140u) allocates 0x458 bytes and calls
// the constructor at RVA 0x89a0 (mfc140u), which initializes private storage
// beyond OpenMFC's 0x418-byte declaration. OpenMFC's own constructor initializes
// the modeled CPane base and derived object; private retail-only operations
// remain unsupported, but returning nullptr would unnecessarily prevent runtime
// class creation for the modeled object.
// Symbol: ?CreateObject@CMFCAutoHideBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCAutoHideBar__SAPEAVCObject__XZ(void)
{
    auto* pBar = new CMFCAutoHideBar();
    std::lock_guard<std::mutex> lock(g_wave2Mutex);
    g_autoHideBarWindows[pBar] = {};
    return pBar;
}
// Retail mfc140u RVA 0x92b0: calls GetWindowRect using hwnd at +0x40 and
// returns CSize(rc.right-rc.left, rc.bottom-rc.top); both layout arguments are
// ignored. The declared GetSafeHwnd path is equivalent. The result buffer follows
// the retail order: this in RCX, hidden CSize* in RDX.
// Symbol: ?CalcFixedLayout@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CMFCAutoHideBar* pThis, CSize* pRet, int /*bStretch*/, int /*bHorz*/)
{
    if (!pRet) return nullptr;
    if (pThis) {
        RECT rc = {};
        HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd && ::GetWindowRect(hWnd, &rc)) {
            new (pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
            return pRet;
        }
    }
    new (pRet) CSize(0, 0);
    return pRet;
}
// Retail mfc140u RVA 0x9310: walks the CObList beginning at +0x3f8 (nodes
// reached through +0x400), calls each button's scalar-deleting-destructor slot
// (vtable byte offset +0x08, edx=1), then calls CObList::RemoveAll. The list and
// retail button objects are unmodeled, so their teardown cannot be reproduced.
// Symbol: ?CleanUpAutoHideButtons@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpAutoHideButtons_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): the CObList at +0x3f8 and its button objects are not
    // modeled; the retail destruction call through vtable byte offset +0x08
    // and subsequent RemoveAll cannot be reproduced.
}
// Retail mfc140u RVA 0x96d0: if AfxIsExtendedFrameClass(pParentWnd) is true,
// tail-calls CPane::Create with its arguments forwarded; otherwise calls
// AfxThrowInvalidArgException. This wrapper preserves that branch and forwarding.
// Symbol: ?Create@CMFCAutoHideBar@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__Create_CMFCAutoHideBar__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CMFCAutoHideBar* pThis, const wchar_t* lpszWindowName, unsigned long dwStyle,
    const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext)
{
    if (!pThis) return FALSE;
    if (!impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(pParentWnd)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
    }
    return impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
        pThis, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
// Retail mfc140u RVA 0x8f00 creates a CMemDC, calls CBasePane::DoPaint (which
// can visibly fill the background), then walks the +0x400 button list and
// dispatches button paint through vtable byte offset +0x50. This pre-existing
// stub omits all of those painting side effects; the absence of modeled button
// state does not make the retail paint a no-op.
// Symbol: ?DoPaint@CMFCAutoHideBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCAutoHideBar__MEAAXPEAVCDC___Z(
    CMFCAutoHideBar* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list, the
    // active-button tracking and the visual-manager vslots; not modeled.
}
// Retail mfc140u RVA 0x8ae0 calls CWnd::Default and returns 0 when the result
// is not -1, otherwise -1 (`setne`/`lea -1(%rcx)`).
// Symbol: ?OnCreate@CMFCAutoHideBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCAutoHideBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCAutoHideBar* pThis, CREATESTRUCTW* /*pCreateStruct*/)
{
    if (!pThis) return -1;
    const __int64 nResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    return (nResult != -1) ? 0 : -1;
}
// Retail mfc140 RVA 0x95b0 (ANSI twin): resolves the button under the point,
// calls SetFocus for its attached window when +0x38 is non-null, then tail-calls
// CPane::OnLButtonDown with the original flags and point. The current body keeps
// only the CPane tail; button lookup/focus require retail button state.
// Symbol: ?OnLButtonDown@CMFCAutoHideBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCAutoHideBar__IEAAXIVCPoint___Z(
    CMFCAutoHideBar* pThis, unsigned int nFlags, CPoint point)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the ButtonFromPoint lookup
    // (+0x400 list, +0x38 of each button) and the SetFocus side effect are
    // not modeled; the CPane::OnLButtonDown tail is kept.
    impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(pThis, nFlags, point);
}
// Retail mfc140 RVA 0x91f0 (ANSI twin): if +0x438 is non-null, calls that
// button's vtable byte offset +0x40 with edx=0, clears +0x438, calls
// UpdateWindow(hwnd) and InvalidateRect(hwnd, nullptr, TRUE), then clears the
// +0x450 flag. The corresponding ANSI IAT slots resolve to USER32 UpdateWindow
// and InvalidateRect; this body does not set or kill a timer. The button
// pointer/flag are not represented in OpenMFC, so this remains a documented stub.
// Symbol: ?OnMouseLeave@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x438 button pointer,
    // its vtable byte offset +0x40, and the +0x450 flag; not modeled.
}
// Retail mfc140 RVA 0x90a0 (ANSI twin): reads the cursor position, converts it
// from screen to client coordinates, and resolves a button with ButtonFromPoint.
// It calls TrackMouseEvent with TME_LEAVE once, uses timer ID 0xec00 with the
// configured delay (killing a prior timer ID stored at +0x448), and updates the
// button pointers/flags at +0x438/+0x440/+0x450. It does not convert the input
// point to screen coordinates or show a tooltip. These fields are unmodeled.
// Symbol: ?OnMouseMove@CMFCAutoHideBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCAutoHideBar__IEAAXIVCPoint___Z(
    CMFCAutoHideBar* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list, the
    // +0x440/+0x448/+0x450 tracking/timer state, and the USER32 tracking calls;
    // not modeled.
}
// Retail mfc140 RVA 0x93e0 (ANSI twin): kills the bar timer whose ID is stored
// at +0x448 and clears that ID, calls CWnd::OnNcDestroy, then calls the virtual
// at vtable byte offset +0x08 with edx=1. The current body keeps only the CWnd
// call; the timer ID and final virtual dispatch are not modeled.
// Symbol: ?OnNcDestroy@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the timer ID at +0x448 and
    // the final virtual call at byte offset +0x08 are not modeled; the
    // CWnd::OnNcDestroy call is kept.
    impl__OnNcDestroy_CWnd__IEAAXXZ(pThis);
}
// Retail mfc140 RVA 0x9430 (ANSI twin): for timer ID 0xec00 it re-reads the
// cursor position and resolves the button, kills/clears the timer ID at +0x448,
// and may show the attached window when the +0x440 state and visibility check
// require it; other timer IDs skip this branch. All paths tail-call CWnd::Default.
// The current body keeps only that Default call.
// Symbol: ?OnTimer@CMFCAutoHideBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCAutoHideBar__IEAAX_K_Z(
    CMFCAutoHideBar* pThis, unsigned __int64 /*nIDEvent*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the 0xec00 timer branch, the
    // +0x400 button list, +0x440/+0x448 state, and attached-window re-show are
    // not modeled; the CWnd::Default tail is kept.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}
// Retail mfc140 RVA 0x8cd0 (ANSI twin): kills/clears the bar timer ID at +0x448,
// searches the +0x3f8 button list for a button whose +0x38 is pPane, and returns
// 0 if absent. On a match it clears +0x430 when that pointer matches the button,
// destroys/removes the button, updates visibility, and returns 1. This body
// always returns FALSE because the retail list and button objects are unmodeled.
// Symbol: ?RemoveAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__RemoveAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane___Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the timer ID at +0x448, the
    // +0x3f8 button list and the +0x430 button pointer; not modeled.
    return FALSE;
}
// Retail mfc140 RVA 0x9600 (ANSI twin): stores bActivate in
// CPane::m_bActiveInGroup (+0x1b4), then walks the group obtained from +0x130
// and calls each other pane's virtual at vtable byte offset +0x530. The group
// container and that dispatch are not represented in OpenMFC.
// Symbol: ?SetActiveInGroup@CMFCAutoHideBar@@UEAAXH@Z
extern "C" void MS_ABI impl__SetActiveInGroup_CMFCAutoHideBar__UEAAXH_Z(
    CMFCAutoHideBar* pThis, int /*bActivate*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x130 group walk and the
    // sibling virtual dispatch at vtable byte offset +0x530; not modeled.
}
// Retail mfc140 RVA 0x8e00 (ANSI twin): finds the +0x400 list entry whose
// button's +0x38 is pPane, calls its virtual at vtable byte offset +0x30 with
// bShow, updates visible state, and returns 1; an absent match returns 0. The
// list/button objects are unmodeled, so this body currently returns FALSE.
// Symbol: ?ShowAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@HH@Z
extern "C" int MS_ABI impl__ShowAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane__HH_Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/, int /*bShow*/,
    int /*bDelay*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list and
    // the per-button virtual at vtable byte offset +0x30; not modeled.
    return FALSE;
}
// Retail mfc140 RVA 0x94f0 (ANSI twin; corresponding mfc140u body begins at
// RVA 0x9470): calls GetWindowRect but does not use that rectangle for the
// result; it initializes *pRet to zero, then overwrites it with each button's
// GetSize() result while walking the +0x400 list (so the last list entry wins).
// It finally dispatches redraw through vtable byte offset +0x480. The pre-existing
// OpenMFC approximation returns the window rectangle size instead, because
// button sizes/list storage are not modeled.
// Symbol: ?StretchPane@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__StretchPane_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CMFCAutoHideBar* pThis, CSize* pRet, int /*nLength*/, int /*bVert*/)
{
    if (!pRet) return nullptr;
    if (pThis) {
        RECT rc = {};
        HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd && ::GetWindowRect(hWnd, &rc)) {
            new (pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
            return pRet;
        }
    }
    // TODO(clean-room): partially transcribed -- copying each button's GetSize
    // result from the +0x400 list and the vtable byte offset +0x480 redraw are
    // not modeled; only the OpenMFC window-rectangle approximation is kept.
    new (pRet) CSize(0, 0);
    return pRet;
}
// Retail mfc140 RVA 0x8ed0 (ANSI twin): walks the +0x400 button list and calls
// each button's virtual at vtable byte offset +0x68 with pPane. The list and
// button objects are unmodeled.
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideBar@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list and
    // the per-button virtual at vtable byte offset +0x68; not modeled.
}
// Retail mfc140 RVA 0x8e50 (ANSI twin): queries IsWindowVisible and
// GetVisibleCount, then drives the +0x128 mini-frame's virtual at vtable byte
// offset +0x550 with the computed show state; if +0x128 is null it throws
// AfxThrowInvalidArgException. The mini-frame member/dispatch are unmodeled.
// Symbol: ?UpdateVisibleState@CMFCAutoHideBar@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateVisibleState_CMFCAutoHideBar__QEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x128 mini-frame and its
    // virtual at vtable byte offset +0x550 and GetVisibleCount helper; not
    // modeled.
}
CMFCAutoHideBar::CMFCAutoHideBar() {
    std::memset(_pad, 0, sizeof(_pad));
}
CMFCAutoHideBar::~CMFCAutoHideBar() {}
