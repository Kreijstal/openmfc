// CToolBar — OpenMFC implementation.
// Sources: cbarcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include <atomic>
#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Bodies marked "transcribed from retail" were decoded from the retail export
// (disas.py; the retail source is bartool.cpp).  Function bodies are
// byte-identical between mfc140.dll and mfc140u.dll; every RVA quoted in the
// comments added with these transcriptions is an mfc140u.dll address (function
// entries unless labelled as a vftable or global).
//
// Sibling thunks (declared with the signature their mangled name describes;
// every definition was located with grep before use).
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                   // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                       // core/window/Thunks.cpp
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(
    CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);                     // core/window/CWnd.cpp
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);                                          // core/window/Thunks.cpp
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);                  // core/runtime/CCmdTarget.cpp
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis);                   // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                     // detail/MfcExceptionsSupport.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszName, const wchar_t* lpszType);                                                  // core/runtime/Globals.cpp
// featurepack/CMFC_misc_stubs.cpp defines this with pointer-typed placeholder
// parameters `(void** p0, void** p1, int p2)` returning void*.  That list is
// ABI-identical to the mangled (HINSTANCE, HRSRC, BOOL) -> HBITMAP, so it is
// declared here exactly as defined (see headerRequests for the retyping).
extern "C" void* MS_ABI impl__AfxLoadSysColorBitmap__YAPEAUHBITMAP____PEAUHINSTANCE____PEAUHRSRC____H_Z(
    void** hInst, void** hRsrc, int bMono);                                                             // featurepack/CMFC_misc_stubs.cpp
extern "C" long MS_ABI impl__GetCommCtrlVersion__YAJPEAK0_Z(unsigned long* pdwMajor, unsigned long* pdwMinor); // featurepack/CMFC_misc_stubs.cpp

// Thunks defined further down in this file that earlier bodies call.
extern "C" void MS_ABI impl__Layout_CToolBar__IEAAXXZ(CToolBar* pThis);
extern "C" __int64 MS_ABI impl__OnSetSizeHelper_CToolBar__IEAA_JAEAVCSize___J_Z(CToolBar* pThis, CSize* pSize, __int64 lParam);
extern "C" int MS_ABI impl__AddReplaceBitmap_CToolBar__QEAAHPEAUHBITMAP_____Z(CToolBar* pThis, HBITMAP hbmImageWell);

namespace {

// Members retail touches, pinned to the offsets the disassembly uses
// (include/openmfc/afxole.h declares them at these offsets; retail agrees:
// AddReplaceBitmap 0x1db7d0 reads m_hbmImageWell at +0x158 and m_sizeImage.cx
// at +0x164, LoadBitmapW 0x1db720 writes m_hRsrcImageWell +0x148 and
// m_hInstImageWell +0x150, OnPaint 0x1dcea0 tests m_bDelayedButtonLayout
// +0x160, OnSetButtonSize 0x1dced0 passes &m_sizeButton = +0x16c).
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CToolBar, m_hRsrcImageWell) == 0x148, "CToolBar::m_hRsrcImageWell");
static_assert(offsetof(CToolBar, m_hInstImageWell) == 0x150, "CToolBar::m_hInstImageWell");
static_assert(offsetof(CToolBar, m_hbmImageWell) == 0x158, "CToolBar::m_hbmImageWell");
static_assert(offsetof(CToolBar, m_bDelayedButtonLayout) == 0x160, "CToolBar::m_bDelayedButtonLayout");
static_assert(offsetof(CToolBar, m_sizeImage) == 0x164, "CToolBar::m_sizeImage");
static_assert(offsetof(CToolBar, m_sizeButton) == 0x16c, "CToolBar::m_sizeButton");

// The commctrl TBBUTTON as retail's helpers address it on x64 (fsState +8,
// fsStyle +9, bReserved +0xa, iString +0x18, 0x20 bytes compared by memcmp in
// _SetButton).  Spelled ::TBBUTTON because CToolBar declares a nested TBBUTTON.
static_assert(offsetof(::TBBUTTON, iBitmap) == 0x00 && offsetof(::TBBUTTON, idCommand) == 0x04 &&
              offsetof(::TBBUTTON, fsState) == 0x08 && offsetof(::TBBUTTON, fsStyle) == 0x09 &&
              offsetof(::TBBUTTON, bReserved) == 0x0a && offsetof(::TBBUTTON, iString) == 0x18 &&
              sizeof(::TBBUTTON) == 0x20, "x64 TBBUTTON layout");

// Message numbers exactly as the retail bodies pass them.  Note 0x415 is
// TB_INSERTBUTTONA: retail _SetButton (0x1dba90) uses that value in mfc140u too.
constexpr UINT kTB_ADDBITMAP       = 0x413;
constexpr UINT kTB_INSERTBUTTON415 = 0x415;
constexpr UINT kTB_DELETEBUTTON    = 0x416;
constexpr UINT kTB_GETBUTTON       = 0x417;
constexpr UINT kTB_BUTTONCOUNT     = 0x418;
constexpr UINT kTB_GETITEMRECT     = 0x41d;
constexpr UINT kTB_REPLACEBITMAP   = 0x42e;
constexpr UINT kTB_SETMAXTEXTROWS  = 0x43c;
constexpr UINT kTB_GETTEXTROWS     = 0x43d;
constexpr UINT kTB_SETEXTENDEDSTYLE = 0x454;
constexpr UINT kTB_GETEXTENDEDSTYLE = 0x455;
// afxext.h TBBS_* (MAKELONG(style, state)) as CToolCmdUI tests them.
constexpr UINT kTBBS_CHECKBOX      = 0x00000002;   // TBSTYLE_CHECK
constexpr UINT kTBBS_CHECKED       = 0x00010000;   // TBSTATE_CHECKED << 16
constexpr UINT kTBBS_PRESSED       = 0x00020000;   // TBSTATE_PRESSED << 16
constexpr UINT kTBBS_DISABLED      = 0x00040000;   // TBSTATE_ENABLED << 16
constexpr UINT kTBBS_INDETERMINATE = 0x00100000;   // TBSTATE_INDETERMINATE << 16
// OnUpdateCmdUI's command codes: CN_UPDATE_COMMAND_UI (-1), CN_COMMAND (0), and
// the reflected MAKELONG(CN_UPDATE_COMMAND_UI, WM_COMMAND + WM_REFLECT_BASE).
constexpr int kCN_UPDATE_COMMAND_UI = -1;
constexpr int kCN_COMMAND = 0;
constexpr int kReflectUpdateCode = static_cast<int>(0xbd11ffffu);
// _afxComCtlVersion thresholds in OnSetSizeHelper: VERSION_IE4 = MAKELONG(71, 4)
// and VERSION_6 = MAKELONG(0, 6).
constexpr long kVERSION_IE4 = 0x40047;
constexpr long kVERSION_6   = 0x60000;

// _afxComCtlVersion (global 0x3b1b94, mfc140u; OnSetSizeHelper reads it
// directly).  DEVIATION: retail keeps one module-wide cache that CToolBar's
// creation path fills; this file keeps its own, filled on first use from the
// exported GetCommCtrlVersion exactly as core/controlbar/CReBar.cpp does, so a
// value is always available instead of the -1 an unfilled retail cache holds.
long ComCtlVersion() {
    static long s_ver = -1;
    if (s_ver == -1) {
        unsigned long major = 0, minor = 0;
        impl__GetCommCtrlVersion__YAJPEAK0_Z(&major, &minor);
        s_ver = static_cast<long>(((major & 0xffffu) << 16) | (minor & 0xffffu));
    }
    return s_ver;
}

// this->DefWindowProc(msg, wParam, lParam): CWnd vslot 73 (`call *0x248(%rax)`)
// in every retail body below.  Retail's CWnd::DefWindowProc calls
// ::CallWindowProc(m_pfnSuper, ...) for a subclassed control; OpenMFC's
// CToolBar::CreateEx (further down in this file) creates the toolbar with
// ::CreateWindowExW and no subclassing, so the HWND's own procedure still IS
// comctl32's and ::SendMessage reaches the same code.  DEVIATION: the call is
// not virtual here, so a client class overriding DefWindowProc is bypassed.
// Same helper as SbDefWindowProc in core/controlbar/CStatusBar.cpp and
// RbDefWindowProc in CReBar.cpp.
inline LRESULT TbDefWindowProc(const CToolBar* pThis, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (pThis->m_hWnd == nullptr) return 0;
    if (pThis->m_pfnSuper != nullptr) {
        return ::CallWindowProcW(pThis->m_pfnSuper, pThis->m_hWnd, msg, wParam, lParam);
    }
    return ::SendMessageW(pThis->m_hWnd, msg, wParam, lParam);
}

// CToolBar::_GetButton (unexported, 0x1dba60 mfc140u), transcribed:
//     DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton);
//     pButton->fsState ^= TBSTATE_ENABLED;       // TBSTATE_ENABLED == TBBS_DISABLED, so invert it
// DEVIATION: the caller's buffer is zeroed first so a failed TB_GETBUTTON
// yields zeros instead of retail's uninitialised stack.
inline void TbGetButton(const CToolBar* pThis, int nIndex, ::TBBUTTON* pButton) {
    std::memset(pButton, 0, sizeof(*pButton));
    TbDefWindowProc(pThis, kTB_GETBUTTON, static_cast<WPARAM>(static_cast<INT_PTR>(nIndex)),
                    reinterpret_cast<LPARAM>(pButton));
    pButton->fsState ^= TBSTATE_ENABLED;
}

// CToolBar::_SetButton (unexported, 0x1dba90 mfc140u), transcribed:
//     TBBUTTON button;  DefWindowProc(TB_GETBUTTON, nIndex, &button);
//     pButton->fsState ^= TBSTATE_ENABLED;
//     button.bReserved[0..1] = 0;  pButton->bReserved[0..1] = 0;   // two 16-bit stores
//     if (memcmp(pButton, &button, sizeof(TBBUTTON)) == 0) return;
//     DWORD dwStyle = GetStyle();                                  // 0x2a9690
//     ModifyStyle(WS_VISIBLE, 0);                                  // 0x2a96f0
//     DefWindowProc(TB_DELETEBUTTON, nIndex, 0);
//     if (pButton->iString < -1) {
//         int iTextRows = ::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0);
//         ::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
//         ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows + 1, 0);
//         ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iTextRows, 0);
//         ::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
//         pButton->iString += 1000000;                             // addq $0xf4240
//     }
//     DefWindowProc(0x415 /*TB_INSERTBUTTONA*/, nIndex, pButton);
//     ModifyStyle(0, dwStyle & WS_VISIBLE);
//     if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
//         ((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
//         ::InvalidateRect(m_hWnd, NULL, TRUE);                    // Invalidate()
//     else {
//         CRect rect(0, 0, 0, 0);
//         if (DefWindowProc(TB_GETITEMRECT, nIndex, &rect))
//             ::InvalidateRect(m_hWnd, &rect, TRUE);
//     }
// The IAT slots were resolved in mfc140u: memcmp, SendMessageW, InvalidateRect.
void TbSetButton(CToolBar* pThis, int nIndex, ::TBBUTTON* pButton) {
    const WPARAM wIndex = static_cast<WPARAM>(static_cast<INT_PTR>(nIndex));
    ::TBBUTTON button;
    std::memset(&button, 0, sizeof(button));   // DEVIATION: retail leaves it uninitialised
    TbDefWindowProc(pThis, kTB_GETBUTTON, wIndex, reinterpret_cast<LPARAM>(&button));
    pButton->fsState ^= TBSTATE_ENABLED;
    button.bReserved[0] = 0;
    button.bReserved[1] = 0;
    pButton->bReserved[0] = 0;
    pButton->bReserved[1] = 0;
    if (std::memcmp(pButton, &button, sizeof(::TBBUTTON)) == 0) return;

    const DWORD dwStyle = impl__GetStyle_CWnd__QEBAKXZ(pThis);
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(pThis, WS_VISIBLE, 0, 0);
    TbDefWindowProc(pThis, kTB_DELETEBUTTON, wIndex, 0);
    if (pButton->iString < -1) {
        const LRESULT iTextRows = ::SendMessageW(pThis->m_hWnd, kTB_GETTEXTROWS, 0, 0);
        ::SendMessageW(pThis->m_hWnd, WM_SETREDRAW, FALSE, 0);
        ::SendMessageW(pThis->m_hWnd, kTB_SETMAXTEXTROWS,
                       static_cast<WPARAM>(static_cast<INT_PTR>(static_cast<int>(iTextRows) + 1)), 0);
        ::SendMessageW(pThis->m_hWnd, kTB_SETMAXTEXTROWS,
                       static_cast<WPARAM>(static_cast<INT_PTR>(static_cast<int>(iTextRows))), 0);
        ::SendMessageW(pThis->m_hWnd, WM_SETREDRAW, TRUE, 0);
        pButton->iString += 1000000;
    }
    TbDefWindowProc(pThis, kTB_INSERTBUTTON415, wIndex, reinterpret_cast<LPARAM>(pButton));
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(pThis, 0, dwStyle & WS_VISIBLE, 0);

    if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
        ((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap)) {
        ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    } else {
        RECT rect = {0, 0, 0, 0};
        if (TbDefWindowProc(pThis, kTB_GETITEMRECT, wIndex, reinterpret_cast<LPARAM>(&rect))) {
            ::InvalidateRect(pThis->m_hWnd, &rect, TRUE);
        }
    }
}

// CToolBar::GetButtonStyle, retail 0x1dbdc0 (mfc140u), transcribed:
//     TBBUTTON button;  _GetButton(nIndex, &button);
//     return MAKELONG(button.fsStyle, button.fsState);
// (The exported thunk in core/controlbar/Thunks.cpp still routes to the C++
// CToolBar::GetButtonStyle below, which returns fsStyle only.)
inline UINT TbGetButtonStyle(const CToolBar* pThis, int nIndex) {
    ::TBBUTTON button;
    TbGetButton(pThis, nIndex, &button);
    return static_cast<UINT>(button.fsStyle) | (static_cast<UINT>(button.fsState) << 16);
}

// CToolBar::SetButtonStyle, retail 0x1dbe00 (mfc140u), transcribed:
//     TBBUTTON button;  _GetButton(nIndex, &button);
//     if (button.fsStyle != (BYTE)LOWORD(nStyle) || button.fsState != (BYTE)HIWORD(nStyle)) {
//         button.fsStyle = (BYTE)LOWORD(nStyle);
//         button.fsState = (BYTE)HIWORD(nStyle);
//         _SetButton(nIndex, &button);
//         m_bDelayedButtonLayout = TRUE;               // movl $1,0x160(%rbx)
//     }
void TbSetButtonStyle(CToolBar* pThis, int nIndex, UINT nStyle) {
    ::TBBUTTON button;
    TbGetButton(pThis, nIndex, &button);
    const BYTE newStyle = static_cast<BYTE>(nStyle);
    const BYTE newState = static_cast<BYTE>(nStyle >> 16);
    if (button.fsStyle != newStyle || button.fsState != newState) {
        button.fsStyle = newStyle;
        button.fsState = newState;
        TbSetButton(pThis, nIndex, &button);
        pThis->m_bDelayedButtonLayout = TRUE;
    }
}

// ---- MSVC-layout CToolCmdUI for OnUpdateCmdUI -------------------------------
// OnUpdateCmdUI (0x1dd4f0) builds a CToolCmdUI on its stack (vftable 0x3227e8,
// mfc140u) and hands it to OnCmdMsg, so the object a client's
// ON_UPDATE_COMMAND_UI handler receives must have the retail CCmdUI layout and
// an MSVC-style vtable.  That vftable is [0x1dd420 CToolCmdUI::Enable,
// 0x1dd480 CToolCmdUI::SetCheck, 0x1dea90 CCmdUI::SetRadio, 0x27d0 SetText].
// OpenMFC's CCmdUI (afxole.h) has a different layout and a virtual destructor,
// and the CToolCmdUI exports in core/cmdui/CToolCmdUI.cpp dispatch through it,
// so the overrides are transcribed here as file-local MS_ABI functions, as
// core/controlbar/CStatusBar.cpp does for CStatusCmdUI.
struct MsCmdUI {
    const void* const* vfptr;   // +0x00
    UINT     m_nID;             // +0x08
    int      m_nIndex;          // +0x0c
    void*    m_pMenu;           // +0x10
    void*    m_pSubMenu;        // +0x18
    void*    m_pOther;          // +0x20  (the CToolBar)
    BOOL     m_bEnableChanged;  // +0x28
    BOOL     m_bContinueRouting;// +0x2c
    UINT     m_nIndexMax;       // +0x30
    void*    m_pParentMenu;     // +0x38
};
static_assert(sizeof(MsCmdUI) == 0x40, "retail CCmdUI is 0x40 bytes");
static_assert(offsetof(MsCmdUI, m_nID) == 0x08 && offsetof(MsCmdUI, m_nIndex) == 0x0c &&
              offsetof(MsCmdUI, m_pOther) == 0x20 && offsetof(MsCmdUI, m_bEnableChanged) == 0x28 &&
              offsetof(MsCmdUI, m_nIndexMax) == 0x30 && offsetof(MsCmdUI, m_pParentMenu) == 0x38,
              "retail CCmdUI offsets");

// CToolCmdUI::Enable(BOOL), retail 0x1dd420 (mfc140u), transcribed:
//     m_bEnableChanged = TRUE;                                        // +0x28
//     CToolBar* pToolBar = (CToolBar*)m_pOther;                       // +0x20, no NULL check
//     UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~TBBS_DISABLED;   // btr $0x12
//     if (!bOn) { nNewStyle &= ~TBBS_PRESSED; nNewStyle |= TBBS_DISABLED; }   // btr $0x11; bts $0x12
//     pToolBar->SetButtonStyle(m_nIndex, nNewStyle);                  // tail jump 0x1dbe00
void MS_ABI CmdUI_Enable(MsCmdUI* self, int bOn) {
    self->m_bEnableChanged = TRUE;
    CToolBar* pToolBar = static_cast<CToolBar*>(self->m_pOther);
    if (pToolBar == nullptr) return;   // retail would fault
    UINT nNewStyle = TbGetButtonStyle(pToolBar, self->m_nIndex) & ~kTBBS_DISABLED;
    if (!bOn) {
        nNewStyle &= ~kTBBS_PRESSED;
        nNewStyle |= kTBBS_DISABLED;
    }
    TbSetButtonStyle(pToolBar, self->m_nIndex, nNewStyle);
}
// CToolCmdUI::SetCheck(int), retail 0x1dd480 (mfc140u), transcribed:
//     UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~(TBBS_CHECKED | TBBS_INDETERMINATE);  // & 0xffeeffff
//     if (nCheck == 1) nNewStyle |= TBBS_CHECKED;
//     else if (nCheck == 2) nNewStyle |= TBBS_INDETERMINATE;
//     pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);  // or $0x2; tail jump 0x1dbe00
void MS_ABI CmdUI_SetCheck(MsCmdUI* self, int nCheck) {
    CToolBar* pToolBar = static_cast<CToolBar*>(self->m_pOther);
    if (pToolBar == nullptr) return;   // retail would fault
    UINT nNewStyle = TbGetButtonStyle(pToolBar, self->m_nIndex) & ~(kTBBS_CHECKED | kTBBS_INDETERMINATE);
    if (nCheck == 1) nNewStyle |= kTBBS_CHECKED;
    else if (nCheck == 2) nNewStyle |= kTBBS_INDETERMINATE;
    TbSetButtonStyle(pToolBar, self->m_nIndex, nNewStyle | kTBBS_CHECKBOX);
}
// Slot 2 is CCmdUI::SetRadio (0x1dea90, mfc140u), which first calls the
// SetCheck virtual (slot 1) with (bOn != 0) and then does menu work only when
// m_pMenu != NULL && m_pSubMenu == NULL.  m_pMenu is always NULL in the
// toolbar's CmdUI, so only the SetCheck forward is transcribed.
void MS_ABI CmdUI_SetRadio(MsCmdUI* self, int bOn) { CmdUI_SetCheck(self, bOn ? 1 : 0); }
// Slot 3, CToolCmdUI::SetText, resolves to 0x27d0 (mfc140u), a bare `ret`
// shared by many empty functions: the toolbar ignores text updates.
void MS_ABI CmdUI_SetText(MsCmdUI* /*self*/, const wchar_t* /*lpszText*/) {}
const void* const g_msToolCmdUIVtbl[4] = {
    reinterpret_cast<const void*>(&CmdUI_Enable),
    reinterpret_cast<const void*>(&CmdUI_SetCheck),
    reinterpret_cast<const void*>(&CmdUI_SetRadio),
    reinterpret_cast<const void*>(&CmdUI_SetText),
};
// CCmdTarget::OnCmdMsg is MSVC vtable slot 5 (`mov 0x28(%rax),%rax` on
// pTarget inside OnUpdateCmdUI); the target frame is reached by raw slot, as
// core/controlbar/CStatusBar.cpp does.
constexpr size_t kVtOnCmdMsgSlot = 5;
using OnCmdMsgFn = int (MS_ABI*)(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);

// The vptr OpenMFC's own (g++-compiled) CToolBar constructor installs,
// captured by CToolBar::CToolBar() further down.  A client that declares a
// plain `CToolBar m_wndToolBar;` gets exactly this vptr, because the exported
// ctor (core/controlbar/Thunks.cpp, `new(pThis) CToolBar()`) builds the object
// with g++'s vtable; a client class DERIVED from CToolBar overwrites it with
// its own MSVC vftable after that ctor returns.  The exported
// CCmdTarget::OnCmdMsg thunk (core/runtime/CCmdTarget.cpp) reads
// GetMessageMap from MSVC vslot 12, and its own comment warns that an object
// still carrying the g++ vtable must not be handed to it: in g++'s CToolBar
// vtable (checked with -fdump-lang-class) slot 12 is CWnd::WindowProc, so the
// call would run WindowProc with garbage arguments and dereference its
// LRESULT as a message map.  OnUpdateCmdUI uses this to tell the two apart.
std::atomic<const void*> g_gxxToolBarVptr{nullptr};
inline bool HasGxxToolBarVtable(const CToolBar* pThis) {
    return *reinterpret_cast<const void* const*>(pThis) ==
           g_gxxToolBarVptr.load(std::memory_order_relaxed);
}

} // namespace

// Symbol: ?CalcDynamicLayout@CToolBar@@UEAA?AVCSize@@HK@Z
extern "C" void MS_ABI impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(
    CSize* pRet, CToolBar* pThis, int nLength, unsigned long dwMode) {
    (void)dwMode;
    CSize size = ToolbarDefaultSize(pThis);
    if (nLength > 0) size.cx = nLength;
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?CalcFixedLayout@CToolBar@@UEAA?AVCSize@@HH@Z
extern "C" void MS_ABI impl__CalcFixedLayout_CToolBar__UEAA_AVCSize__HH_Z(
    CSize* pRet, CToolBar* pThis, int bStretch, int bHorz) {
    CSize size = ToolbarDefaultSize(pThis);
    if (bStretch && pThis && pThis->GetSafeHwnd()) {
        RECT rc = {};
        if (::GetClientRect(::GetParent(pThis->GetSafeHwnd()), &rc)) {
            if (bHorz) size.cx = rc.right - rc.left;
            else size.cy = rc.bottom - rc.top;
        }
    }
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?CalcLayout@CToolBar@@IEAA?AVCSize@@KH@Z
extern "C" void MS_ABI impl__CalcLayout_CToolBar__IEAA_AVCSize__KH_Z(
    CSize* pRet, CToolBar* pThis, unsigned long dwMode, int nLength) {
    impl__CalcDynamicLayout_CToolBar__UEAA_AVCSize__HK_Z(pRet, pThis, nLength, dwMode);
}
// Symbol: ?CalcSize@CToolBar@@IEAA?AVCSize@@PEAU_TBBUTTON@@H@Z
extern "C" void MS_ABI impl__CalcSize_CToolBar__IEAA_AVCSize__PEAU_TBBUTTON__H_Z(
    CSize* pRet, CToolBar* pThis, TBBUTTON* pData, int nCount) {
    (void)pData;
    CSize size = ToolbarDefaultSize(pThis, nCount);
    BuildCSizeResult(pRet, size.cx, size.cy);
}
// Symbol: ?get_accName@CToolBar@@UEAAJUtagVARIANT@@PEAPEA_W@Z
extern "C" long MS_ABI impl__get_accName_CToolBar__UEAAJUtagVARIANT__PEAPEA_W_Z(
    CToolBar* pThis, VARIANT varChild, wchar_t** pszName) {
    if (!pszName) return E_POINTER;
    *pszName = nullptr;
    int index = (varChild.vt == VT_I4) ? (int)varChild.lVal - 1 : -1;
    CString text = pThis && index >= 0 ? pThis->GetButtonText(index) : CString(L"Toolbar");
    *pszName = ::SysAllocString((const wchar_t*)text);
    return *pszName ? S_OK : E_OUTOFMEMORY;
}
// GetMessageMap/GetThisMessageMap for CToolBar live in core/controlbar/MessageMaps.cpp
// (classCToolBar_msgmap, base map CControlBar). Retail agrees: ?GetThisMessageMap@
// CToolBar@@KAPEBUAFX_MSGMAP@@XZ in mfc140.dll returns the AFX_MSGMAP at 0x180320648,
// whose pfnGetBaseMap is ?GetThisMessageMap@CControlBar@@KAPEBUAFX_MSGMAP@@XZ. The copies
// that used to be here returned CWnd's map, skipping CToolBar and CControlBar entirely.
// Symbol: ?GetRuntimeClass@CToolBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBar__UEBAPEAUCRuntimeClass__XZ(const CToolBar* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CToolBar::GetThisClass();
}
// Symbol: ?GetThisClass@CToolBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ() {
    return CToolBar::GetThisClass();
}
// Retail CToolBar::Layout (0x1dbd60, mfc140u) first clears
// m_bDelayedButtonLayout (`movl $0,0x160(%rcx)`) and then calls
// CalcDynamicLayout (vslot 92) with LM_* flags chosen from m_dwStyle.  Only the
// flag clear is transcribed here (so OnPaint's delayed layout runs once, as in
// retail); the TB_AUTOSIZE below is OpenMFC's pre-existing stand-in for the
// CalcDynamicLayout call, which is itself not yet a retail transcription.
// Symbol: ?Layout@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__Layout_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis) pThis->m_bDelayedButtonLayout = FALSE;
    if (pThis && pThis->GetSafeHwnd()) {
        ::SendMessageW(pThis->GetSafeHwnd(), TB_AUTOSIZE, 0, 0);
    }
}
// Symbol: ?OnBarStyleChange@CToolBar@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnBarStyleChange_CToolBar__UEAAXKK_Z(CToolBar* pThis, unsigned long oldStyle, unsigned long newStyle) {
    (void)oldStyle;
    if (pThis) pThis->SetBarStyle(newStyle);
}
// Symbol: ?OnEraseBkgnd@CToolBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CToolBar__IEAAHPEAVCDC___Z(CToolBar* pThis, CDC* pDC) {
    if (!pThis || !pThis->GetSafeHwnd() || !pDC || !pDC->GetSafeHdc()) return FALSE;
    return static_cast<int>(::DefWindowProcW(
        pThis->GetSafeHwnd(), WM_ERASEBKGND,
        reinterpret_cast<WPARAM>(pDC->GetSafeHdc()), 0));
}
// Symbol: ?OnNcCalcSize@CToolBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CToolBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    CToolBar* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpncsp) return;
    ::DefWindowProcW(pThis->GetSafeHwnd(), WM_NCCALCSIZE,
                     static_cast<WPARAM>(bCalcValidRects != FALSE),
                     reinterpret_cast<LPARAM>(lpncsp));
}
// Symbol: ?OnNcCreate@CToolBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(CToolBar* pThis, CREATESTRUCTW* lpCreateStruct) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpCreateStruct) return FALSE;
    return static_cast<int>(::DefWindowProcW(
        pThis->GetSafeHwnd(), WM_NCCREATE, 0,
        reinterpret_cast<LPARAM>(lpCreateStruct)) != FALSE);
}
// Symbol: ?OnNcHitTest@CToolBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CToolBar__IEAA_JVCPoint___Z(CToolBar* pThis, CPoint point) {
    return pThis && pThis->GetSafeHwnd() ? ::DefWindowProcW(pThis->GetSafeHwnd(), WM_NCHITTEST, 0, MAKELPARAM(point.x, point.y)) : HTCLIENT;
}
// Symbol: ?OnPreserveSizingPolicyHelper@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPreserveSizingPolicyHelper_CToolBar__IEAA_J_K_J_Z(CToolBar* pThis, unsigned __int64, __int64) {
    return pThis ? pThis->GetBarStyle() : 0;
}
// Symbol: ?OnPreserveZeroBorderHelper@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnPreserveZeroBorderHelper_CToolBar__IEAA_J_K_J_Z(
    CToolBar* pThis, unsigned __int64, __int64) {
    return pThis ? pThis->GetBarStyle() : 0;
}
// Transcribed from retail CToolBar::OnSetButtonSize, RVA 0x1dced0 (mfc140u;
// TB_SETBUTTONSIZE handler):
//     return OnSetSizeHelper(m_sizeButton, lParam);     // lea 0x16c(%rcx),%rdx; tail jump 0x1dcef0
// (Replaces an earlier body that called SetSizes, which re-sends
// TB_SETBUTTONSIZE from inside the TB_SETBUTTONSIZE handler.)
// Symbol: ?OnSetButtonSize@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetButtonSize_CToolBar__IEAA_J_K_J_Z(CToolBar* pThis, unsigned __int64, __int64 lParam) {
    if (pThis == nullptr) return 0;
    return impl__OnSetSizeHelper_CToolBar__IEAA_JAEAVCSize___J_Z(
        pThis, reinterpret_cast<CSize*>(&pThis->m_sizeButton), lParam);
}
// Transcribed from retail CToolBar::OnSetSizeHelper, RVA 0x1dcef0 (mfc140u):
//     BOOL bModify = FALSE;  DWORD dwStyle = 0;  DWORD dwStyleEx = 0;
//     if (_afxComCtlVersion >= VERSION_IE4) {                   // 0x40047, signed compare
//         dwStyle = GetStyle();                                  // 0x2a9690
//         bModify = ModifyStyle(0, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT);   // 0x8800; 0x2a96f0
//         if (_afxComCtlVersion >= VERSION_6 && ::IsWindow(GetSafeHwnd())) {   // 0x60000
//             DWORD ex = ::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0);
//             dwStyleEx = ::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, ex & ~TBSTYLE_EX_DRAWDDARROWS);
//         }                                                      // (dwStyleEx = the previous extended style)
//     }
//     LRESULT lResult = Default();                               // 0x28ac80
//     if (lResult) size = DWORD(lParam);                         // cx = (short)LOWORD, cy = (short)HIWORD
//     if (bModify) ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
//     if (dwStyleEx) ::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwStyleEx);
//     return lResult;
// IAT slots resolved in mfc140u: IsWindow, SendMessageW, SetWindowLongW.
// (Replaces an earlier body that always wrote *pSize into m_sizeButton.)
// Symbol: ?OnSetSizeHelper@CToolBar@@IEAA_JAEAVCSize@@_J@Z
extern "C" __int64 MS_ABI impl__OnSetSizeHelper_CToolBar__IEAA_JAEAVCSize___J_Z(CToolBar* pThis, CSize* pSize, __int64 lParam) {
    if (pThis == nullptr || pSize == nullptr) return 0;
    BOOL bModify = FALSE;
    DWORD dwStyle = 0;
    DWORD dwStyleEx = 0;
    if (ComCtlVersion() >= kVERSION_IE4) {
        dwStyle = impl__GetStyle_CWnd__QEBAKXZ(pThis);
        bModify = impl__ModifyStyle_CWnd__QEAAHKKI_Z(pThis, 0, TBSTYLE_TRANSPARENT | TBSTYLE_FLAT, 0);
        if (ComCtlVersion() >= kVERSION_6 && ::IsWindow(pThis->GetSafeHwnd())) {
            const LRESULT ex = ::SendMessageW(pThis->m_hWnd, kTB_GETEXTENDEDSTYLE, 0, 0);
            dwStyleEx = static_cast<DWORD>(::SendMessageW(
                pThis->m_hWnd, kTB_SETEXTENDEDSTYLE, 0,
                ex & static_cast<LPARAM>(0xfffffffeu) /* ~TBSTYLE_EX_DRAWDDARROWS */));
        }
    }
    const __int64 lResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    if (lResult) {
        pSize->cx = static_cast<short>(LOWORD(static_cast<DWORD>(lParam)));
        pSize->cy = static_cast<short>(HIWORD(static_cast<DWORD>(lParam)));
    }
    if (bModify) ::SetWindowLongW(pThis->m_hWnd, GWL_STYLE, static_cast<LONG>(dwStyle));
    if (dwStyleEx) ::SendMessageW(pThis->m_hWnd, kTB_SETEXTENDEDSTYLE, 0, static_cast<LPARAM>(dwStyleEx));
    return lResult;
}
// Symbol: ?OnSysColorChange@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Symbol: ?OnToolHitTest@CToolBar@@UEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CToolBar__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CToolBar* pThis, CPoint point, TOOLINFOW* pTI) {
    if (!pThis || !pThis->GetSafeHwnd()) return -1;
    int count = (int)::SendMessageW(pThis->GetSafeHwnd(), TB_BUTTONCOUNT, 0, 0);
    for (int i = 0; i < count; ++i) {
        RECT rc = {};
        if (::SendMessageW(pThis->GetSafeHwnd(), TB_GETITEMRECT, i, (LPARAM)&rc) && ::PtInRect(&rc, POINT{point.x, point.y})) {
            if (pTI) {
                pTI->hwnd = pThis->GetSafeHwnd();
                pTI->uId = (UINT_PTR)pThis->GetItemID(i);
                pTI->rect = rc;
            }
            return i;
        }
    }
    return -1;
}
// Transcribed from retail CToolBar::OnUpdateCmdUI, RVA 0x1dd4f0 (mfc140u):
//     CToolCmdUI state;                                  // vftable 0x3227e8; CCmdUI ctor inlined (all zero)
//     state.m_pOther = this;
//     state.m_nIndexMax = (UINT)DefWindowProc(TB_BUTTONCOUNT, 0, 0);   // vslot 73
//     for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++) {
//         TBBUTTON button;  _GetButton(state.m_nIndex, &button);        // 0x1dba60
//         state.m_nID = button.idCommand;
//         if (button.fsStyle & TBSTYLE_SEP) continue;                   // ignore separators
//         // allow reflections
//         if (CWnd::OnCmdMsg(0, MAKELONG(CN_UPDATE_COMMAND_UI, WM_COMMAND + WM_REFLECT_BASE), &state, NULL))
//             continue;                                                 // 0x1de460 = CCmdTarget::OnCmdMsg, direct call
//         // allow the toolbar itself to have update handlers
//         if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
//             continue;
//         // allow the owner to process the update -- CCmdUI::DoUpdate(pTarget, bDisableIfNoHndler) inlined:
//         if (state.m_nID == 0 || LOWORD(state.m_nID) == 0xFFFF) continue;
//         ENSURE_ARG(pTarget != NULL);                                  // AfxThrowInvalidArgException, 0x227720
//         state.m_bEnableChanged = FALSE;
//         pTarget->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL);          // vslot 5
//         if (bDisableIfNoHndler && !state.m_bEnableChanged) {
//             AFX_CMDHANDLERINFO info;  info.pTarget = NULL;
//             BOOL bHandler = pTarget->OnCmdMsg(state.m_nID, CN_COMMAND, &state, &info);   // vslot 5
//             state.Enable(bHandler);    // vtable slot 0; devirtualised to CToolCmdUI::Enable when it matches
//         }
//     }
//     // update the dialog controls added to the toolbar
//     UpdateDialogControls(pTarget, bDisableIfNoHndler);                 // 0x291460
// Here the vtable is g_msToolCmdUIVtbl, so state.Enable is CmdUI_Enable.
// DEVIATION: the two "toolbar handles it itself" OnCmdMsg calls are skipped
// when `this` still carries OpenMFC's g++ CToolBar vtable (a plain,
// non-derived CToolBar; see g_gxxToolBarVptr).  For such an object retail's
// calls return FALSE anyway: the retail message maps on that chain --
// CToolBar 0x322808, CControlBar 0x3212a8, CWnd 0x336c60 (mfc140u; the root
// CCmdTarget map is never scanned) -- hold no WM_COMMAND entry, so neither
// the reflected nor the plain CN_UPDATE_COMMAND_UI lookup can match.
// pTarget is reached through MSVC vslot 5, which is right for a frame whose
// vftable is MSVC's (any client-derived frame); an OpenMFC-constructed,
// non-derived frame would carry g++'s vtable instead (same limitation as
// CStatusBar::OnUpdateCmdUI in core/controlbar/CStatusBar.cpp).
// Symbol: ?OnUpdateCmdUI@CToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CToolBar__UEAAXPEAVCFrameWnd__H_Z(CToolBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;
    MsCmdUI state;
    std::memset(&state, 0, sizeof(state));
    state.vfptr = g_msToolCmdUIVtbl;
    state.m_pOther = pThis;
    state.m_nIndexMax = static_cast<UINT>(TbDefWindowProc(pThis, kTB_BUTTONCOUNT, 0, 0));
    const bool bSelfDispatch = !HasGxxToolBarVtable(pThis);
    for (state.m_nIndex = 0; static_cast<UINT>(state.m_nIndex) < state.m_nIndexMax; ++state.m_nIndex) {
        ::TBBUTTON button;
        TbGetButton(pThis, state.m_nIndex, &button);
        state.m_nID = static_cast<UINT>(button.idCommand);
        if (button.fsStyle & TBSTYLE_SEP) continue;
        if (bSelfDispatch &&
            impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(pThis, 0, kReflectUpdateCode, &state, nullptr)) {
            continue;
        }
        if (bSelfDispatch &&
            impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(pThis, state.m_nID, kCN_UPDATE_COMMAND_UI, &state, nullptr)) {
            continue;
        }
        if (state.m_nID == 0 || static_cast<WORD>(state.m_nID) == 0xFFFF) continue;
        if (pTarget == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        state.m_bEnableChanged = FALSE;
        void** vtbl = *reinterpret_cast<void***>(pTarget);
        OnCmdMsgFn fnOnCmdMsg = reinterpret_cast<OnCmdMsgFn>(vtbl[kVtOnCmdMsgSlot]);
        fnOnCmdMsg(pTarget, state.m_nID, kCN_UPDATE_COMMAND_UI, &state, nullptr);
        if (bDisableIfNoHndler && !state.m_bEnableChanged) {
            void* info[2] = {nullptr, nullptr};   // AFX_CMDHANDLERINFO {pTarget, pmf}; retail zeroes pTarget only
            const int bHandler = fnOnCmdMsg(pTarget, state.m_nID, kCN_COMMAND, &state, info);
            CmdUI_Enable(&state, bHandler);
        }
    }
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisableIfNoHndler);
}
// Symbol: ?OnWindowPosChanging@CToolBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CToolBar__IEAAXPEAUtagWINDOWPOS___Z(CToolBar* pThis, WINDOWPOS* lpWndPos) {
    if (!pThis || !pThis->GetSafeHwnd() || !lpWndPos) return;
    ::DefWindowProcW(pThis->GetSafeHwnd(), WM_WINDOWPOSCHANGING, 0,
                     reinterpret_cast<LPARAM>(lpWndPos));
}
// Symbol: ?SetOwner@CToolBar@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__SetOwner_CToolBar__QEAAXPEAVCWnd___Z(CToolBar* pThis, CWnd* pOwner) {
    if (pThis && pThis->GetSafeHwnd()) {
        ::SetWindowLongPtrW(pThis->GetSafeHwnd(), GWLP_HWNDPARENT, (LONG_PTR)(pOwner ? pOwner->GetSafeHwnd() : nullptr));
    }
}
// Symbol: ?SizeToolBar@CToolBar@@IEAAXPEAU_TBBUTTON@@HHH@Z
extern "C" void MS_ABI impl__SizeToolBar_CToolBar__IEAAXPEAU_TBBUTTON__HHH_Z(
    CToolBar* pThis, TBBUTTON* pData, int nCount, int nLength, int bVert) {
    (void)pData;
    if (!pThis) return;
    CSize sz = ToolbarDefaultSize(pThis, nCount);
    if (nLength > 0) {
        if (bVert) sz.cy = nLength;
        else sz.cx = nLength;
    }
    pThis->m_sizeButton = sz;
}
// Symbol: ?WrapToolBar@CToolBar@@IEAAHPEAU_TBBUTTON@@HH@Z
extern "C" int MS_ABI impl__WrapToolBar_CToolBar__IEAAHPEAU_TBBUTTON__HH_Z(
    CToolBar* pThis, TBBUTTON* pData, int nCount, int nWidth) {
    (void)pThis;
    if (!pData || nCount <= 0 || nWidth <= 0) return 0;
    int rows = 1;
    int x = 0;
    for (int i = 0; i < nCount; ++i) {
        x += 23;
        if (x > nWidth) {
            pData[i].fsState |= TBSTATE_WRAP;
            x = 23;
            ++rows;
        }
    }
    return rows;
}
// Symbol: ?GetButtonText@CToolBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* pRet, const CToolBar* pThis, int nIndex) {
    new (pRet) CString(pThis->GetButtonText(nIndex));
}
// Symbol: ?GetButtonText@CToolBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetButtonText_CToolBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CToolBar* pThis, int nIndex, CString* pStr) {
    pThis->GetButtonText(nIndex, *pStr);
}
CToolBar::CToolBar()
    : m_hRsrcImageWell(nullptr), m_hInstImageWell(nullptr), m_hbmImageWell(nullptr),
      m_bDelayedButtonLayout(FALSE), m_pStringMap(nullptr) {
    // Record the g++ vptr this ctor installs (see g_gxxToolBarVptr).
    g_gxxToolBarVptr.store(*reinterpret_cast<const void* const*>(this), std::memory_order_relaxed);
    // m_nCount and the docking members are inherited from CControlBar (ctor).
    m_sizeButton.cx = 23;
    m_sizeButton.cy = 22;
    m_sizeImage.cx = 16;
    m_sizeImage.cy = 15;
}
CToolBar::~CToolBar() {
    if (m_hbmImageWell) {
        ::DeleteObject(m_hbmImageWell);
        m_hbmImageWell = nullptr;
    }
}
BOOL CToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    return CreateEx(pParentWnd, TBSTYLE_FLAT, dwStyle, CRect(0,0,0,0), nID);
}
BOOL CToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle,
                         CRect rcBorders, UINT nID) {
    if (!pParentWnd) return FALSE;

    m_dwStyle = dwStyle;

    DWORD dwWinStyle = dwStyle & 0xFFFF;
    dwWinStyle |= WS_CHILD | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER;

    m_hWnd = ::CreateWindowExW(0, TOOLBARCLASSNAMEW, nullptr, dwWinStyle,
                                rcBorders.left, rcBorders.top,
                                rcBorders.right - rcBorders.left,
                                rcBorders.bottom - rcBorders.top,
                                pParentWnd->GetSafeHwnd(), (HMENU)(UINT_PTR)nID,
                                AfxGetInstanceHandle(), nullptr);

    if (!m_hWnd) return FALSE;

    ::SendMessageW(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwCtrlStyle);
    ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(m_sizeImage.cx, m_sizeImage.cy));
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));

    return TRUE;
}
BOOL CToolBar::LoadToolBar(UINT nIDResource) {
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(),
                                   MAKEINTRESOURCEW(nIDResource), RT_TOOLBAR);
    if (!hRsrc) return FALSE;

    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;

    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wVersion = pData[0];
    WORD wCount = pData[1];
    WORD wWidth = pData[2];
    WORD wHeight = pData[3];
    (void)wVersion; (void)wWidth; (void)wHeight;

    WORD* pButtonIDs = pData + 4;

    for (WORD i = 0; i < wCount; ++i) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}
BOOL CToolBar::LoadToolBar(const wchar_t* lpszResourceName) {
    if (!lpszResourceName) return FALSE;
    HRSRC hRsrc = ::FindResourceW(AfxGetInstanceHandle(), lpszResourceName, RT_TOOLBAR);
    if (!hRsrc) return FALSE;
    HGLOBAL hGlobal = ::LoadResource(AfxGetInstanceHandle(), hRsrc);
    if (!hGlobal) return FALSE;
    WORD* pData = (WORD*)::LockResource(hGlobal);
    if (!pData) return FALSE;

    WORD wCount = pData[1];
    WORD* pButtonIDs = pData + 4;

    for (int i = 0; i < wCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = pButtonIDs[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (pButtonIDs[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_ADDBUTTONSW, 1, (LPARAM)&tb);
    }

    m_nCount = wCount;
    return TRUE;
}
BOOL CToolBar::LoadBitmap(UINT nIDResource) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), MAKEINTRESOURCEW(nIDResource));
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}
BOOL CToolBar::LoadBitmap(const wchar_t* lpszResourceName) {
    HBITMAP hbm = ::LoadBitmapW(AfxGetInstanceHandle(), lpszResourceName);
    if (!hbm) return FALSE;
    return SetBitmap(hbm);
}
BOOL CToolBar::SetButtons(const UINT* lpIDArray, int nIDCount) {
    if (!m_hWnd || !lpIDArray || nIDCount <= 0) return FALSE;

    while ((int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0) > 0) {
        ::SendMessageW(m_hWnd, TB_DELETEBUTTON, 0, 0);
    }

    for (int i = 0; i < nIDCount; i++) {
        TBBUTTON tb = {};
        tb.iBitmap = I_IMAGENONE;
        tb.idCommand = lpIDArray[i];
        tb.fsState = TBSTATE_ENABLED;
        tb.fsStyle = (lpIDArray[i] == 0) ? BTNS_SEP : BTNS_BUTTON;
        tb.dwData = 0;
        tb.iString = -1;
        ::SendMessageW(m_hWnd, TB_INSERTBUTTONW, i, (LPARAM)&tb);
    }
    m_nCount = nIDCount;
    return TRUE;
}
BOOL CToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage) {
    if (!m_hWnd) return FALSE;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_COMMAND | TBIF_STYLE | TBIF_IMAGE;
    tbi.idCommand = nID;
    tbi.fsStyle = (BYTE)nStyle;
    tbi.iImage = iImage;
    return ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi) ? TRUE : FALSE;
}
void CToolBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const {
    nID = 0; nStyle = 0; iImage = -1;
    if (!m_hWnd) return;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb)) {
        nID = tb.idCommand;
        nStyle = tb.fsStyle;
        iImage = tb.iBitmap;
    }
}
int CToolBar::CommandToIndex(UINT nIDFind) const {
    if (!m_hWnd) return -1;
    return (int)::SendMessageW(m_hWnd, TB_COMMANDTOINDEX, nIDFind, 0);
}
UINT CToolBar::GetItemID(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.idCommand;
    return 0;
}
void CToolBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (m_hWnd && lpRect)
        ::SendMessageW(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)lpRect);
}
void CToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage) {
    m_sizeButton = sizeButton;
    m_sizeImage = sizeImage;
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(sizeImage.cx, sizeImage.cy));
        ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(sizeButton.cx, sizeButton.cy));
    }
}
CSize CToolBar::GetButtonSize() const {
    return CSize(m_sizeButton.cx, m_sizeButton.cy);
}
SIZE CToolBar::GetToolBarCtrlSize() const {
    SIZE sz = {0, 0};
    if (m_hWnd) ::SendMessageW(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)&sz);
    return sz;
}
void CToolBar::SetHeight(int cyHeight) {
    if (!m_hWnd) return;
    m_sizeButton.cy = cyHeight;
    ::SendMessageW(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(m_sizeButton.cx, m_sizeButton.cy));
}
int CToolBar::GetHeight() const {
    return m_sizeButton.cy;
}
void CToolBar::SetButtonStyle(int nIndex, UINT nStyle) {
    if (!m_hWnd) return;
    TBBUTTONINFO tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_STYLE;
    tbi.fsStyle = (BYTE)nStyle;
    ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi);
}
UINT CToolBar::GetButtonStyle(int nIndex) const {
    if (!m_hWnd) return 0;
    TBBUTTON tb = {};
    if (::SendMessageW(m_hWnd, TB_GETBUTTON, nIndex, (LPARAM)&tb))
        return tb.fsStyle;
    return 0;
}
BOOL CToolBar::SetButtonText(int nIndex, const wchar_t* lpszText) {
    if (!m_hWnd) return FALSE;
    TBBUTTONINFOW tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFOW);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = (LPWSTR)lpszText;
    return static_cast<BOOL>(::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, nIndex, (LPARAM)&tbi));
}
CString CToolBar::GetButtonText(int nIndex) const {
    CString str;
    if (!m_hWnd) return str;
    wchar_t buf[256] = {};
    TBBUTTONINFOW tbi = {};
    tbi.cbSize = sizeof(TBBUTTONINFOW);
    tbi.dwMask = TBIF_TEXT;
    tbi.pszText = buf;
    tbi.cchText = 256;
    if (::SendMessageW(m_hWnd, TB_GETBUTTONINFOW, nIndex, (LPARAM)&tbi))
        str = buf;
    return str;
}
void CToolBar::GetButtonText(int nIndex, CString& rString) const {
    rString = GetButtonText(nIndex);
}
BOOL CToolBar::SetBitmap(HBITMAP hbmImageWell) {
    if (!m_hWnd || !hbmImageWell) return FALSE;
    if (m_hbmImageWell) ::DeleteObject(m_hbmImageWell);
    m_hbmImageWell = hbmImageWell;

    TBADDBITMAP tbAddBmp = {};
    tbAddBmp.hInst = nullptr;
    tbAddBmp.nID = (UINT_PTR)hbmImageWell;
    ::SendMessageW(m_hWnd, TB_ADDBITMAP, 1, (LPARAM)&tbAddBmp);

    int nCount = (int)::SendMessageW(m_hWnd, TB_BUTTONCOUNT, 0, 0);
    for (int i = 0; i < nCount; i++) {
        TBBUTTONINFO tbi = {};
        tbi.cbSize = sizeof(TBBUTTONINFO);
        tbi.dwMask = TBIF_IMAGE;
        tbi.iImage = i;
        ::SendMessageW(m_hWnd, TB_SETBUTTONINFOW, i, (LPARAM)&tbi);
    }
    return TRUE;
}
void CToolBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC keeps no tooltip member; the toolbar control owns it.
    if (m_hWnd && pToolTip && pToolTip->GetSafeHwnd()) {
        ::SendMessageW(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pToolTip->GetSafeHwnd(), 0);
    }
}
CToolTipCtrl* CToolBar::GetToolTips() const {
    if (!m_hWnd) return nullptr;
    HWND h = (HWND)::SendMessageW(m_hWnd, TB_GETTOOLTIPS, 0, 0);
    if (!h) return nullptr;
    return reinterpret_cast<CToolTipCtrl*>(CWnd::FromHandle(h));
}
void CToolBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}
BOOL CToolBar::IsVisible() const {
    return m_hWnd && (::GetWindowLongW(m_hWnd, GWL_STYLE) & WS_VISIBLE);
}
BOOL CToolBar::IsFloating() const {
    return (m_dwStyle & CBRS_FLOATING) != 0;
}

// === Moved from ManualThunks.cpp ===
// Transcribed from retail CToolBar::GetButtonInfo, RVA 0x1dc790 (mfc140u):
//     TBBUTTON button;  _GetButton(nIndex, &button);      // 0x1dba60
//     nID = button.idCommand;
//     nStyle = MAKELONG(button.fsStyle, button.fsState);  // fsState already XOR TBSTATE_ENABLED
//     iImage = button.iBitmap;
// (Replaces an auto-generated placeholder whose parameter list was
// `(void* pThis, void* p0, void* p1, void* p2, void* p3)`.)
// Symbol: ?GetButtonInfo@CToolBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetButtonInfo_CToolBar__QEBAXHAEAI0AEAH_Z(
    const CToolBar* pThis, int nIndex, UINT* pnID, UINT* pnStyle, int* piImage) {
    if (pThis == nullptr || pnID == nullptr || pnStyle == nullptr || piImage == nullptr) return;
    ::TBBUTTON button;
    TbGetButton(pThis, nIndex, &button);
    *pnID = static_cast<UINT>(button.idCommand);
    *pnStyle = static_cast<UINT>(button.fsStyle) | (static_cast<UINT>(button.fsState) << 16);
    *piImage = button.iBitmap;
}

// Transcribed from retail CToolBar::AddReplaceBitmap, RVA 0x1db7d0 (mfc140u):
//     BITMAP bitmap;  ::GetObject(hbmImageWell, sizeof(BITMAP), &bitmap);   // IAT GetObjectW; result unchecked
//     int nImages = bitmap.bmWidth / m_sizeImage.cx;                        // idiv, before the branch
//     BOOL bResult;
//     if (m_hbmImageWell == NULL) {                                         // +0x158
//         TBADDBITMAP addBitmap = { NULL, (UINT_PTR)hbmImageWell };
//         bResult = DefWindowProc(TB_ADDBITMAP, nImages, &addBitmap) == 0;  // vslot 73; sete
//     } else {
//         TBREPLACEBITMAP replaceBitmap = { NULL, (UINT_PTR)m_hbmImageWell,
//                                           NULL, (UINT_PTR)hbmImageWell, nImages };
//         bResult = (BOOL)DefWindowProc(TB_REPLACEBITMAP, 0, &replaceBitmap);
//     }
//     if (bResult) {
//         AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);   // inlined: if (*p) ::DeleteObject(*p)
//         m_hbmImageWell = hbmImageWell;
//     }
//     return bResult;
// DEVIATIONS: `bitmap` is zeroed first (retail reads uninitialised stack when
// GetObject fails), and a zero m_sizeImage.cx returns FALSE where retail's
// idiv would raise a divide-by-zero fault.
// (Replaces an auto-generated placeholder `(void** p0)` that had no `this`.)
// Symbol: ?AddReplaceBitmap@CToolBar@@QEAAHPEAUHBITMAP__@@@Z
extern "C" int MS_ABI impl__AddReplaceBitmap_CToolBar__QEAAHPEAUHBITMAP_____Z(CToolBar* pThis, HBITMAP hbmImageWell) {
    if (pThis == nullptr) return FALSE;
    BITMAP bitmap;
    std::memset(&bitmap, 0, sizeof(bitmap));
    ::GetObjectW(hbmImageWell, sizeof(BITMAP), &bitmap);
    if (pThis->m_sizeImage.cx == 0) return FALSE;
    const int nImages = bitmap.bmWidth / pThis->m_sizeImage.cx;

    BOOL bResult;
    if (pThis->m_hbmImageWell == nullptr) {
        TBADDBITMAP addBitmap;
        addBitmap.hInst = nullptr;   // makes TBADDBITMAP::nID behave as an HBITMAP
        addBitmap.nID = reinterpret_cast<UINT_PTR>(hbmImageWell);
        bResult = TbDefWindowProc(pThis, kTB_ADDBITMAP,
                                  static_cast<WPARAM>(static_cast<INT_PTR>(nImages)),
                                  reinterpret_cast<LPARAM>(&addBitmap)) == 0;
    } else {
        TBREPLACEBITMAP replaceBitmap;
        replaceBitmap.hInstOld = nullptr;
        replaceBitmap.nIDOld = reinterpret_cast<UINT_PTR>(pThis->m_hbmImageWell);
        replaceBitmap.hInstNew = nullptr;
        replaceBitmap.nIDNew = reinterpret_cast<UINT_PTR>(hbmImageWell);
        replaceBitmap.nButtons = nImages;
        bResult = static_cast<BOOL>(TbDefWindowProc(pThis, kTB_REPLACEBITMAP, 0,
                                                    reinterpret_cast<LPARAM>(&replaceBitmap)));
    }
    if (bResult) {
        if (pThis->m_hbmImageWell != nullptr) ::DeleteObject(pThis->m_hbmImageWell);
        pThis->m_hbmImageWell = hbmImageWell;
    }
    return bResult;
}

// Transcribed from retail CToolBar::LoadBitmapW, RVA 0x1db720 (mfc140u):
//     HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);   // 0x2aeb50
//     HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);   // IAT FindResourceW
//     if (hRsrcImageWell == NULL) return FALSE;
//     HBITMAP hbmImageWell = AfxLoadSysColorBitmap(hInstImageWell, hRsrcImageWell, FALSE);  // 0x1dae20
//     if (!AddReplaceBitmap(hbmImageWell)) return FALSE;                                    // 0x1db7d0
//     m_hInstImageWell = hInstImageWell;    // +0x150
//     m_hRsrcImageWell = hRsrcImageWell;    // +0x148
//     return TRUE;
// (Replaces an auto-generated placeholder `(const wchar_t* p0)` that had no `this`.)
// Symbol: ?LoadBitmapW@CToolBar@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadBitmapW_CToolBar__QEAAHPEB_W_Z(CToolBar* pThis, const wchar_t* lpszResourceName) {
    if (pThis == nullptr) return FALSE;
    HINSTANCE hInstImageWell = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszResourceName, RT_BITMAP);
    HRSRC hRsrcImageWell = ::FindResourceW(hInstImageWell, lpszResourceName, RT_BITMAP);
    if (hRsrcImageWell == nullptr) return FALSE;
    HBITMAP hbmImageWell = static_cast<HBITMAP>(
        impl__AfxLoadSysColorBitmap__YAPEAUHBITMAP____PEAUHINSTANCE____PEAUHRSRC____H_Z(
            reinterpret_cast<void**>(hInstImageWell), reinterpret_cast<void**>(hRsrcImageWell), FALSE));
    if (!impl__AddReplaceBitmap_CToolBar__QEAAHPEAUHBITMAP_____Z(pThis, hbmImageWell)) return FALSE;
    pThis->m_hInstImageWell = hInstImageWell;
    pThis->m_hRsrcImageWell = hRsrcImageWell;
    return TRUE;
}

// Retail CToolBar::OnNcPaint (WM_NCPAINT handler) resolves to RVA 0x1d9430
// (mfc140u), the same folded body as CStatusBar::OnNcPaint:
//     EraseNonClient();                                  // tail jump 0x1d6e50
// Symbol: ?OnNcPaint@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis == nullptr) return;
    impl__EraseNonClient_CControlBar__QEAAXXZ(pThis);
}

// Transcribed from retail CToolBar::OnPaint, RVA 0x1dcea0 (mfc140u; WM_PAINT handler):
//     if (m_bDelayedButtonLayout)                        // +0x160
//         Layout();                                      // 0x1dbd60
//     Default();                                         // tail jump 0x28ac80
// Symbol: ?OnPaint@CToolBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CToolBar__IEAAXXZ(CToolBar* pThis) {
    if (pThis == nullptr) return;
    if (pThis->m_bDelayedButtonLayout) impl__Layout_CToolBar__IEAAXXZ(pThis);
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Transcribed from retail CToolBar::OnSetBitmapSize, RVA 0x1dcee0 (mfc140u;
// TB_SETBITMAPSIZE handler):
//     return OnSetSizeHelper(m_sizeImage, lParam);       // lea 0x164(%rcx),%rdx; tail jump 0x1dcef0
// (Replaces an auto-generated placeholder `(unsigned __int64 p0, __int64 p1)`
// that had no `this`.)
// Symbol: ?OnSetBitmapSize@CToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetBitmapSize_CToolBar__IEAA_J_K_J_Z(CToolBar* pThis, unsigned __int64 /*wParam*/, __int64 lParam) {
    if (pThis == nullptr) return 0;
    return impl__OnSetSizeHelper_CToolBar__IEAA_JAEAVCSize___J_Z(
        pThis, reinterpret_cast<CSize*>(&pThis->m_sizeImage), lParam);
}
