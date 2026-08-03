// OpenMFC: CMFCAutoHideBar exports.
//
// All 17 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// CMFCAutoHideBar is declared in include/openmfc/afxmfc.h as a thin CPane
// subclass with 32 bytes of opaque padding; the retail object is 0x458 bytes.
// The internal storage the retail bodies use is unmodeled: the auto-hide-button
// list (+0x3f8 CObList head / +0x400 list head), the active-window pointers
// (+0x430/+0x438/+0x448), the tracking/timer flags (+0x440/+0x450) and the
// group index (+0x1b4).  Everything reachable through the declared public API
// (the CWnd base with m_hWnd at +0x40, GetSafeHwnd, RedrawWindow) is kept; the
// rest is reduced to the retail failure terminal with a `// TODO(clean-room)`
// marker.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstdint>
#include <cstring>

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
extern "C" int MS_ABI impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(
    CWnd* pThis, unsigned int nFlags, CPoint point);
extern "C" int MS_ABI impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(
    CWnd* pWnd);
// AfxThrowInvalidArgException is only exported as an extern-C MS_ABI impl_
// function (mfc_exceptions.cpp); the C++ symbol the header declares has no
// definition, so the impl_ form is called directly (same ABI result).
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

//=============================================================================
// CMFCAutoHideBar - declared as a thin CPane subclass; only the CWnd base
// (hwnd at +0x40) and the declared public API are modeled.
//=============================================================================

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CreateObject
//-----------------------------------------------------------------------------
// Retail (0x180008950) allocates a 0x458-byte object (operator new at
// 0x1800027f0) and runs the constructor (0x1800089a0), which installs the
// retail vftable (0x1802db808) and initializes the unmodeled members.  The
// declared OpenMFC class has no modeled constructor state or vftable, so no
// valid object can be produced; the factory conservatively fails like a
// failed allocation.
// Symbol: ?CreateObject@CMFCAutoHideBar@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCAutoHideBar__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x458 bytes
    // and runs the CMFCAutoHideBar constructor (RVA 0x1800089a0, retail
    // vftable at 0x1802db808); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CalcFixedLayout
//-----------------------------------------------------------------------------
// Retail (0x1800092b0): GetWindowRect(hwnd@+0x40, &rc) (IAT 0x1802c6c08),
// returns CSize(rc.right-rc.left, rc.bottom-rc.top).  Both layout arguments
// are ignored.  The declared GetSafeHwnd path is equivalent.
// Symbol: ?CalcFixedLayout@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CMFCAutoHideBar* pThis, int /*bStretch*/, int /*bHorz*/)
{
    if (!pRet) return;
    if (pThis) {
        RECT rc = {};
        HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd && ::GetWindowRect(hWnd, &rc)) {
            new (pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
            return;
        }
    }
    new (pRet) CSize(0, 0);
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::CleanUpAutoHideButtons
//-----------------------------------------------------------------------------
// Retail (0x180009310): walks the auto-hide-button list at +0x3f8 (CObList),
// destroys every button window via its vslot-0x58 virtual (edx=1) and then
// tail-calls CObList::RemoveAll (0x180008350).  The list and the button
// virtual are unmodeled, so no teardown can be driven.
// Symbol: ?CleanUpAutoHideButtons@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__CleanUpAutoHideButtons_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the auto-hide-button CObList
    // at +0x3f8 and the per-button vslot-0x58 destruction virtual are not
    // modeled; the retail teardown cannot be reproduced.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::Create
//-----------------------------------------------------------------------------
// Retail (0x1800096d0): if AfxIsExtendedFrameClass(pParentWnd) (0x18006c840)
// is true, tail-calls CPane::Create (0x18009f4f0) with all arguments
// forwarded; otherwise calls AfxThrowInvalidArgException (0x180227720).  Both
// call targets are exports, so the body is transcribed faithfully.
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

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::DoPaint
//-----------------------------------------------------------------------------
// Retail (0x180008f00): creates a CMemDC (0x180069f50), delegates the base
// fill to CBasePane::DoPaint (0x18000b1b0), then walks the button list at
// +0x400 and re-runs the active button's vslot-0x50 paint virtual.  The list,
// the active-button bookkeeping and the visual-manager dispatch are all
// unmodeled; the paint has no other visible side effect.
// Symbol: ?DoPaint@CMFCAutoHideBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCAutoHideBar__MEAAXPEAVCDC___Z(
    CMFCAutoHideBar* pThis, CDC* /*pDC*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list, the
    // active-button tracking and the visual-manager vslots; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnCreate
//-----------------------------------------------------------------------------
// Retail (0x180008ae0): calls CWnd::Default (0x18028ac80); returns 0 when the
// default result != -1 and -1 otherwise (`setne`/`lea -1(%rcx)`).
// Symbol: ?OnCreate@CMFCAutoHideBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCAutoHideBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCAutoHideBar* pThis, CREATESTRUCTW* /*pCreateStruct*/)
{
    if (!pThis) return -1;
    const __int64 nResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    return (nResult != -1) ? 0 : -1;
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnLButtonDown
//-----------------------------------------------------------------------------
// Retail (0x180009530): resolves the button under the point (ButtonFromPoint
// 0x1800091d0), gives the attached window the focus (SetFocus 0x1802a9b60)
// when its +0x38 member is set, then tail-calls CPane::OnLButtonDown
// (0x18009f810) with (nFlags, point) forwarded.  ButtonFromPoint walks the
// unmodeled +0x400 list, so only the base-class tail is kept.
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

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnMouseLeave
//-----------------------------------------------------------------------------
// Retail (0x180009170): when the +0x438 auto-hide-window pointer is set,
// invokes its vslot-0x40 virtual (edx=0), kills a timer (IAT 0x1802c7300),
// arms the 1ms leave timer (IAT 0x1802c7128) and clears +0x438; then stores
// 0 into the +0x450 tracking flag.  The pointers and timers are unmodeled.
// Symbol: ?OnMouseLeave@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x438 auto-hide window
    // and its vslot-0x40, the timer IDs and the +0x450 flag; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnMouseMove
//-----------------------------------------------------------------------------
// Retail (0x180009020): converts the point to screen coords, resolves the
// button under the point, arms the 0x18/2 tracking timer, destroys the
// previous auto-hide window (+0x448), drives the tooltip/pane-show vslots and
// the +0x440/+0x450 tracking flags.  All of that state is unmodeled.
// Symbol: ?OnMouseMove@CMFCAutoHideBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCAutoHideBar__IEAAXIVCPoint___Z(
    CMFCAutoHideBar* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list, the
    // +0x440/+0x448/+0x450 tracking state and the tooltip/timer machinery;
    // not modeled.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnNcDestroy
//-----------------------------------------------------------------------------
// Retail (0x180009360): when the +0x448 auto-hide-window pointer is set,
// destroys that window and clears the pointer; then calls CWnd::OnNcDestroy
// (0x18028b900) and tail-calls the vslot-0x58 base handler (edx=1).  The
// +0x448 window is unmodeled; the CWnd::OnNcDestroy call is kept.
// Symbol: ?OnNcDestroy@CMFCAutoHideBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCAutoHideBar__IEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the auto-hide window at
    // +0x448 and its DestroyWindow teardown are not modeled; the
    // CWnd::OnNcDestroy base call is kept.
    impl__OnNcDestroy_CWnd__IEAAXXZ(pThis);
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::OnTimer
//-----------------------------------------------------------------------------
// Retail (0x1800093b0): handles only the 0xec00 timer; it re-resolves the
// button under the cursor, destroys the previous auto-hide window (+0x448),
// and when the tracked button's window becomes visible shows it again
// (ShowAttachedWindow 0x18000a3f0).  The timer and the +0x440/+0x448 state
// are unmodeled; the CWnd::Default tail (0x18028ac80) is kept.
// Symbol: ?OnTimer@CMFCAutoHideBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCAutoHideBar__IEAAX_K_Z(
    CMFCAutoHideBar* pThis, unsigned __int64 /*nIDEvent*/)
{
    if (!pThis) return;
    // TODO(clean-room): partially transcribed -- the 0xec00 timer branch, the
    // +0x400/+0x448 tracking and the ShowAttachedWindow re-show are not
    // modeled; the CWnd::Default tail is kept.
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::RemoveAutoHideWindow
//-----------------------------------------------------------------------------
// Retail (0x180008c50): destroys the +0x448 auto-hide window and clears the
// pointer, walks the +0x3f8 list for the button whose +0x38 == pPane, clears
// the +0x430 active-window pointer when it matches and removes the record
// (CObList::RemoveAt 0x1802320d0).  The lists and pointers are unmodeled, so
// the retail failure terminal 0 is returned.
// Symbol: ?RemoveAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@@Z
extern "C" int MS_ABI impl__RemoveAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane___Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the +0x448 window, the +0x3f8
    // button list and the +0x430 active-window pointer; not modeled.
    return FALSE;
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::SetActiveInGroup
//-----------------------------------------------------------------------------
// Retail (0x180009580): stores bActivate into +0x1b4, then walks the +0x130
// dock-site group (GetGroupFromPane 0x180052530) and calls each sibling
// bar's vslot-0x70 virtual with bActivate.  The group index, the dock-site
// group and the sibling virtual are unmodeled.
// Symbol: ?SetActiveInGroup@CMFCAutoHideBar@@UEAAXH@Z
extern "C" void MS_ABI impl__SetActiveInGroup_CMFCAutoHideBar__UEAAXH_Z(
    CMFCAutoHideBar* pThis, int /*bActivate*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x1b4 group index, the
    // +0x130 dock-site group walk and the sibling vslot-0x70; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::ShowAutoHideWindow
//-----------------------------------------------------------------------------
// Retail (0x180008d80): walks the +0x400 list for the button whose +0x38 ==
// pPane, calls its vslot-0x30 virtual with bShow, then runs
// UpdateVisibleState (0x180008dd0) and returns 1.  The list and the button
// virtual are unmodeled, so the retail failure terminal 0 is returned.
// Symbol: ?ShowAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@HH@Z
extern "C" int MS_ABI impl__ShowAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane__HH_Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/, int /*bShow*/,
    int /*bDelay*/)
{
    if (!pThis) return FALSE;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list and
    // the per-button vslot-0x30; not modeled.
    return FALSE;
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::StretchPane
//-----------------------------------------------------------------------------
// Retail (0x180009470): fills the return CSize with 0, then walks the +0x400
// list and takes the largest GetSize.cx; afterwards it dispatches a
// stretch-redraw through the vslot-0x480 virtual.  The list is unmodeled, so
// only the window-rect measurement (GetWindowRect IAT 0x1802c6c08) is kept.
// Symbol: ?StretchPane@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__StretchPane_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CMFCAutoHideBar* pThis, int /*nLength*/, int /*bVert*/)
{
    if (!pRet) return;
    if (pThis) {
        RECT rc = {};
        HWND hWnd = pThis->GetSafeHwnd();
        if (hWnd && ::GetWindowRect(hWnd, &rc)) {
            new (pRet) CSize(rc.right - rc.left, rc.bottom - rc.top);
            return;
        }
    }
    // TODO(clean-room): partially transcribed -- the +0x400 button-list walk
    // (max GetSize.cx) and the vslot-0x480 stretch redraw are not modeled;
    // only the window-rect measurement is kept.
    new (pRet) CSize(0, 0);
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::UnSetAutoHideMode
//-----------------------------------------------------------------------------
// Retail (0x180008e50): walks the +0x400 button list and calls each button's
// vslot-0x68 virtual with pPane.  The list and the virtual are unmodeled.
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideBar@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(
    CMFCAutoHideBar* pThis, CDockablePane* /*pPane*/)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x400 button list and
    // the per-button vslot-0x68; not modeled.
}

//-----------------------------------------------------------------------------
// CMFCAutoHideBar::UpdateVisibleState
//-----------------------------------------------------------------------------
// Retail (0x180008dd0): queries IsWindowVisible(hwnd) (IAT 0x1802c7328) and
// GetVisibleCount (0x180008e90), then drives the +0x128 mini-frame's
// vslot-0x550 virtual with the computed show state; when +0x128 is NULL it
// calls AfxThrowInvalidArgException (0x180227720).  The +0x128 member and the
// virtual are unmodeled.
// Symbol: ?UpdateVisibleState@CMFCAutoHideBar@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateVisibleState_CMFCAutoHideBar__QEAAXXZ(
    CMFCAutoHideBar* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): not transcribed -- needs the +0x128 mini-frame and its
    // vslot-0x550 and the GetVisibleCount helper; not modeled.
}
