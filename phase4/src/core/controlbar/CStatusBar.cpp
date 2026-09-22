// CStatusBar — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// Method: every body marked "transcribed from retail" below was decoded from
// the mfc140u.dll export (disas.py --u; the retail source is barstat.cpp), the
// way core/ole/COleControl.cpp and core/controlbar/CControlBar.cpp do.  The
// message handlers are not in the RVA symbol map; they were located through
// CStatusBar's retail message map (AFX_MSGMAP at 0x3222b8, entries from
// 0x3222d0, mfc140u):
//    WM_NCHITTEST -> 0x1da8f0   WM_NCPAINT -> 0x1d9430   WM_PAINT -> 0x1daa00
//    WM_NCCALCSIZE -> 0x1da910  WM_SIZE -> 0x1daa30      WM_WINDOWPOSCHANGING -> 0x1d94e0
//    WM_SETTEXT -> 0x1daa60     WM_GETTEXT -> 0x1daab0   WM_GETTEXTLENGTH -> 0x1dab90
//    SB_SETMINHEIGHT (0x408) -> 0x1dabd0
// and the non-exported virtuals through the CStatusBar vftable at 0x322438
// (mfc140u): slot 27 PreCreateWindow (0x1d9f40), 75 OnChildNotify (0x1da9d0),
// 91 CalcFixedLayout (0x1da7b0), 93 OnUpdateCmdUI (0x1daca0), 94 CalcInsideRect
// (0x1da1a0), 105 OnBarStyleChange (0x1da990), 108 Create (0x1d9e40), 109
// CreateEx (0x1d9e70), 110 DrawItem (0x27d0 = a bare `ret`).  UpdateAllPanes
// (0x1da250) is the callee of OnSize / OnPaint / SetIndicators.
// SetIndicators (0x1d9f70), SetPaneStyle (0x1da520), SetPaneInfo (0x1da5c0)
// and SetPaneText (0x1da6b0) are exported by mfc140u but unresolved in the wf2
// RVA map, so they were pinned as call targets instead: SetPaneStyle is the
// tail jump of both CStatusCmdUI::Enable (0x1dac00) and ::SetCheck (0x1dac40),
// SetPaneText the tail jump of CStatusCmdUI::SetText (0x1dac80) and the callee
// of OnSetText / UpdateAllPanes / SetPaneStyle / SetPaneInfo, and SetIndicators
// the only caller of AllocElements(n, 0x18) that then LoadStrings each pane.
//
// Layout: the class is declared for real in include/openmfc/afxole.h (CControlBar
// members at 0xe8..0x140, m_nMinHeight at 0x148, sizeof 0x150) and this file
// keeps the typed style (CStatusBar* pThis).  The pane array m_pData points at is
// retail's AFX_STATUSPANE (barstat.cpp), pinned below from the retail
// AllocElements / SetIndicators / UpdateAllPanes bodies.
//
// Virtual dispatch: OpenMFC's CStatusBar objects carry a mingw (Itanium) vtable
// whose slot numbering is not MSVC's, so wherever retail calls through a vtable
// slot the bodies below call the impl__ thunk directly ("vslot NN" in the
// comment), as core/controlbar/CControlBar.cpp does.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include <cstddef>
#include <cstring>
#include <cwchar>

// ---------------------------------------------------------------------------
// Sibling thunks (declared with the signature their mangled name describes;
// every definition was located with grep before use).
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                   // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);                       // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);       // core/window/CWnd.cpp
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
    const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);                             // core/window/CWnd.cpp
extern "C" int MS_ABI impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(
    CWnd* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam, __int64* pResult);      // core/window/Thunks.cpp
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);                                          // core/window/Thunks.cpp
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);                        // core/runtime/CCmdTarget.cpp (stub, see OnUpdateCmdUI)
extern "C" int MS_ABI impl__AllocElements_CControlBar__QEAAHHH_Z(CControlBar* pThis, int nCount, int nSize); // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(
    const CControlBar* pThis, CRect& rect, int bHorz);                                                  // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__EraseNonClient_CControlBar__QEAAXXZ(CControlBar* pThis);                   // core/controlbar/CControlBar.cpp
extern "C" void MS_ABI impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(
    CControlBar* pThis, WINDOWPOS* pWndPos);                                                            // core/controlbar/CControlBar.cpp
extern "C" int MS_ABI impl__PreCreateWindow_CControlBar__UEAAHAEAUtagCREATESTRUCTW___Z(
    CControlBar* pThis, CREATESTRUCTW& cs);                                                             // core/controlbar/CControlBar.cpp
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);                         // featurepack/CMFC_misc_stubs.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);   // featurepack/CMFC_misc_stubs.cpp
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);                                                          // core/collections/CStringT.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                     // detail/MfcExceptionsSupport.cpp

// Thunks defined further down in this file that earlier bodies call.
extern "C" int MS_ABI impl__AllocElements_CStatusBar__QEAAHHH_Z(CStatusBar* pThis, int nElements, int cbElement);
extern "C" void MS_ABI impl__CalcInsideRect_CStatusBar__UEBAXAEAVCRect__H_Z(const CStatusBar* pThis, CRect& rect, int bHorz);
extern "C" void MS_ABI impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(CStatusBar* pThis, int bUpdateRects, int bUpdateText);
extern "C" int MS_ABI impl__CreateEx_CStatusBar__UEAAHPEAVCWnd__KKI_Z(CStatusBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, unsigned int nID);

namespace {

// Members retail touches, pinned to the offsets the disassembly uses --
// except m_pfnSuper, which is the one member below where OpenMFC and retail
// disagree: retail reads it at +0xb0 (`mov 0xb0(%rcx),%rcx` at the head of
// CWnd::DefWindowProc, 0x28bb80, mfc140u) while OpenMFC's CWnd (afxwin.h:592)
// puts it at +0x48.  Nothing in this file passes the field across the ABI
// boundary, so the assert just pins OpenMFC's own layout for SbDefWindowProc.
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(CWnd, m_pfnSuper) == 0x48, "CWnd::m_pfnSuper");
static_assert(offsetof(CControlBar, m_cxLeftBorder) == 0xf4, "CControlBar::m_cxLeftBorder");
static_assert(offsetof(CControlBar, m_cxRightBorder) == 0xf8, "CControlBar::m_cxRightBorder");
static_assert(offsetof(CControlBar, m_cyTopBorder) == 0xfc, "CControlBar::m_cyTopBorder");
static_assert(offsetof(CControlBar, m_cyBottomBorder) == 0x100, "CControlBar::m_cyBottomBorder");
static_assert(offsetof(CControlBar, m_nCount) == 0x10c, "CControlBar::m_nCount");
static_assert(offsetof(CControlBar, m_pData) == 0x110, "CControlBar::m_pData");
static_assert(offsetof(CControlBar, m_dwStyle) == 0x124, "CControlBar::m_dwStyle");
static_assert(offsetof(CStatusBar, m_nMinHeight) == 0x148, "CStatusBar::m_nMinHeight");
static_assert(sizeof(CStatusBar) == 0x150, "CStatusBar size");

// AFX_STATUSPANE (barstat.cpp), the element m_pData holds m_nCount of.  Retail
// AllocElements (0x1da0e0, mfc140u) steps the array by 0x18 and destroys /
// nil-initialises the CString at +0x10; SetIndicators (0x1d9f70) writes nID at
// +0, cxText at +4, ORs SBPF_UPDATE into nFlags at +0xc and SBPS_STRETCH |
// SBPS_NOBORDERS into nStyle at +8; UpdateAllPanes (0x1da250) reads cxText (+4),
// nStyle (+8), nFlags (+0xc) and strText (+0x10).
struct StatusPane {
    UINT    nID;        // +0x00  IDC of indicator: 0 => normal text area
    int     cxText;     // +0x04  width of string area in pixels
    UINT    nStyle;     // +0x08  style flags (SBPS_*)
    UINT    nFlags;     // +0x0c  state flags (SBPF_*)
    CString strText;    // +0x10  text in the pane
};
static_assert(offsetof(StatusPane, nID) == 0x00, "AFX_STATUSPANE::nID");
static_assert(offsetof(StatusPane, cxText) == 0x04, "AFX_STATUSPANE::cxText");
static_assert(offsetof(StatusPane, nStyle) == 0x08, "AFX_STATUSPANE::nStyle");
static_assert(offsetof(StatusPane, nFlags) == 0x0c, "AFX_STATUSPANE::nFlags");
static_assert(offsetof(StatusPane, strText) == 0x10, "AFX_STATUSPANE::strText");
static_assert(sizeof(StatusPane) == 0x18, "AFX_STATUSPANE size");

// Pane style bits (afxext.h SBPS_*), as the disassembly tests them: SetPaneStyle /
// SetPaneInfo test bit 27 (SBPS_STRETCH), SetPaneText bit 26 (SBPS_DISABLED),
// SetIndicators ORs 0x8000100 (SBPS_STRETCH | SBPS_NOBORDERS) into pane 0,
// CStatusCmdUI::SetCheck toggles bit 9 (SBPS_POPOUT).
constexpr UINT kSBPS_NOBORDERS = 0x00000100;
constexpr UINT kSBPS_POPOUT    = 0x00000200;
constexpr UINT kSBPS_DISABLED  = 0x04000000;
constexpr UINT kSBPS_STRETCH   = 0x08000000;
// barstat.cpp: SBPF_UPDATE (nFlags bit 0) and CX_PANE_BORDER (the 6 in
// UpdateAllPanes' `mov $0xfffffffa,%eax; sub cxText,%eax`).
constexpr UINT kSBPF_UPDATE     = 0x0001;
constexpr int  kCX_PANE_BORDER  = 6;
// Retail CBRS_* bits (afxres.h), NOT the include/openmfc/afxole.h macros (see
// the note in core/controlbar/CControlBar.cpp): CBRS_ALL = 0x0040ffff,
// CBRS_BORDER_ANY = 0x0f00, CBRS_BORDER_3D = 0x80, CBRS_ALIGN_ANY = 0xf000,
// CBRS_BOTTOM = CBRS_ALIGN_BOTTOM | CBRS_BORDER_TOP = 0x8200.
constexpr DWORD kCBRS_ALL        = 0x0040ffff;
constexpr DWORD kCBRS_BORDER_3D  = 0x00000080;
constexpr DWORD kCBRS_BORDER_ANY = 0x00000f00;
constexpr DWORD kCBRS_ALIGN_ANY  = 0x0000f000;
constexpr DWORD kCBRS_BOTTOM     = 0x00008200;
// Common-control bits retail ORs into the window style in CreateEx
// (CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE = 0x4e,
// SBARS_SIZEGRIP = 0x100) and the AfxEndDeferRegisterClass argument
// (AFX_WNDCOMMCTLS_REG = 0x1000).
constexpr DWORD kCCS_STATUSBAR_BITS  = 0x0000004e;
constexpr DWORD kSBARS_SIZEGRIP      = 0x00000100;
constexpr long  kAFX_WNDCOMMCTLS_REG = 0x1000;
// CN_UPDATE_COMMAND_UI (afxwin.h) as OnUpdateCmdUI passes it (`or $-1,%r8d`).
constexpr int kCN_UPDATE_COMMAND_UI = -1;

inline StatusPane* PanePtr(const CStatusBar* pThis, int nIndex) {
    return reinterpret_cast<StatusPane*>(pThis->m_pData) + nIndex;
}
// Retail's _GetPanePtr has only a debug ASSERT on the index; every public
// entry below that takes an index keeps the same unchecked read except where
// the pre-existing OpenMFC body already guarded it (noted per body).
inline bool PaneIndexValid(const CStatusBar* pThis, int nIndex) {
    return pThis->m_pData != nullptr && nIndex >= 0 && nIndex < pThis->m_nCount;
}

// CWnd::DefWindowProc (CWnd vslot 73, retail ?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z
// at RVA 0x28bb80, mfc140u) for the SB_* messages retail routes around its own
// message map.  Retail's body has three branches, in order:
//     if (m_pfnSuper /* +0xb0 */)          ::CallWindowProcW(m_pfnSuper, m_hWnd, ...)
//     else if (*GetSuperWndProcAddr())     ::CallWindowProcW(that, m_hWnd, ...)   // vslot 65
//     else                                 ::DefWindowProcW(m_hWnd, ...)
// For a CStatusBar created through retail's CWnd::Create the first branch is
// the live one: the CBT hook subclassed the comctl32 status-bar procedure and
// parked it in m_pfnSuper, so ::CallWindowProc reaches comctl32.
// OpenMFC's CWnd::Create thunk (core/window/CWnd.cpp) does not subclass the
// window at all, so m_pfnSuper is normally NULL and the HWND's own procedure
// still IS the comctl32 status-bar procedure; ::SendMessage therefore reaches
// the same code with no CWnd dispatch in between.  Retail's third branch
// (::DefWindowProc) is deliberately NOT reproduced: with no subclassing it
// would hand SB_* to the default window procedure, which drops every one of
// them.  The m_pfnSuper branch is kept for a window that was subclassed
// explicitly (CWnd::SubclassWindow), which is the only way it becomes non-NULL
// in this tree.
inline LRESULT SbDefWindowProc(const CStatusBar* pThis, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (pThis->m_hWnd == nullptr) return 0;
    if (pThis->m_pfnSuper != nullptr) {
        return ::CallWindowProcW(pThis->m_pfnSuper, pThis->m_hWnd, msg, wParam, lParam);
    }
    return ::SendMessageW(pThis->m_hWnd, msg, wParam, lParam);
}

// The `for (i = 0; i < m_nCount; i++) if (pane[i].nID == 0) ...` scan that
// OnSetText (0x1daa60), OnGetText (0x1daab0) and OnGetTextLength (0x1dab90)
// inline (CommandToIndex(0)); -1 when no pane has ID 0.
inline int FindMessagePane(const CStatusBar* pThis) {
    for (int i = 0; i < pThis->m_nCount; ++i) {
        if (PanePtr(pThis, i)->nID == 0) return i;
    }
    return -1;
}

// SB_GETBORDERS through DefWindowProc, as UpdateAllPanes / CalcFixedLayout /
// CalcInsideRect do (`mov $0x407,%edx ; call *0x248(%rax)`).
inline void GetSbBorders(const CStatusBar* pThis, int rgBorders[3]) {
    rgBorders[0] = rgBorders[1] = rgBorders[2] = 0;
    SbDefWindowProc(pThis, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(rgBorders));
}

// ---- MSVC-layout CStatusCmdUI for OnUpdateCmdUI ---------------------------
// OnUpdateCmdUI (0x1daca0) builds a CStatusCmdUI on its stack (vftable
// 0x322298, mfc140u) and hands it to the target's OnCmdMsg, so the object a
// client's ON_UPDATE_COMMAND_UI handler receives must have the retail CCmdUI
// layout and an MSVC-style vtable [Enable, SetCheck, SetRadio, SetText].
// OpenMFC's CCmdUI (afxole.h:68) has a different layout and a virtual
// destructor, and the CStatusCmdUI exports in core/cmdui/CStatusCmdUI.cpp cast
// to it, so the overrides are transcribed here as file-local MS_ABI functions
// (same approach as featurepack/controls/CMFCStatusBar.cpp).
struct MsCmdUI {
    const void* const* vfptr;   // +0x00
    UINT     m_nID;             // +0x08
    int      m_nIndex;          // +0x0c
    void*    m_pMenu;           // +0x10
    void*    m_pSubMenu;        // +0x18
    void*    m_pOther;          // +0x20  (the CStatusBar)
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

// CStatusCmdUI::Enable(BOOL) -- retail RVA 0x1dac00 (mfc140u), transcribed:
//     m_bEnableChanged = TRUE;
//     CStatusBar* pBar = (CStatusBar*)m_pOther;                 // +0x20, no NULL check
//     UINT nStyle = pBar->_GetPanePtr(m_nIndex)->nStyle & ~SBPS_DISABLED;   // btr $0x1a
//     if (!bOn) nStyle |= SBPS_DISABLED;
//     pBar->SetPaneStyle(m_nIndex, nStyle);                     // tail jump 0x1da520
void MS_ABI CmdUI_Enable(MsCmdUI* self, int bOn)
{
    self->m_bEnableChanged = TRUE;
    CStatusBar* pBar = static_cast<CStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail would fault
    UINT nStyle = PanePtr(pBar, self->m_nIndex)->nStyle & ~kSBPS_DISABLED;
    if (!bOn) nStyle |= kSBPS_DISABLED;
    pBar->SetPaneStyle(self->m_nIndex, nStyle);
}
// CStatusCmdUI::SetCheck(int) -- retail RVA 0x1dac40: same shape with
// SBPS_POPOUT (bit 9) set when nCheck != 0, and no m_bEnableChanged write.
void MS_ABI CmdUI_SetCheck(MsCmdUI* self, int nCheck)
{
    CStatusBar* pBar = static_cast<CStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail would fault
    UINT nStyle = PanePtr(pBar, self->m_nIndex)->nStyle & ~kSBPS_POPOUT;
    if (nCheck) nStyle |= kSBPS_POPOUT;
    pBar->SetPaneStyle(self->m_nIndex, nStyle);
}
// CStatusCmdUI does not override SetRadio: vftable slot 2 is CCmdUI::SetRadio
// (retail RVA 0x1dea90, mfc140u), which calls the SetCheck virtual (slot 1)
// with (bOn != 0) and then returns unless (m_pMenu != NULL && m_pSubMenu ==
// NULL) -- m_pMenu is always NULL for the status bar's CmdUI, so only the
// SetCheck forward is transcribed.
void MS_ABI CmdUI_SetRadio(MsCmdUI* self, int bOn) { CmdUI_SetCheck(self, bOn ? 1 : 0); }
// CStatusCmdUI::SetText(LPCTSTR) -- retail RVA 0x1dac80:
//     ((CStatusBar*)m_pOther)->SetPaneText(m_nIndex, lpszText, TRUE);   // tail jump 0x1da6b0
void MS_ABI CmdUI_SetText(MsCmdUI* self, const wchar_t* lpszText)
{
    CStatusBar* pBar = static_cast<CStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail would fault
    pBar->SetPaneText(self->m_nIndex, lpszText, TRUE);
}
const void* const g_msCmdUIVtbl[4] = {
    reinterpret_cast<const void*>(&CmdUI_Enable),
    reinterpret_cast<const void*>(&CmdUI_SetCheck),
    reinterpret_cast<const void*>(&CmdUI_SetRadio),
    reinterpret_cast<const void*>(&CmdUI_SetText),
};
// CCmdTarget::OnCmdMsg is MSVC vtable slot 5 (retail calls it as `mov
// 0x28(%rax),%rax` at 0x1dad60 inside OnUpdateCmdUI); the target is a
// client-constructed frame with an MSVC vtable, so it is reached by raw slot,
// as featurepack/controls/CMFCStatusBar.cpp does.
constexpr size_t kVtOnCmdMsgSlot = 5;
using OnCmdMsgFn = int (MS_ABI*)(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);

} // namespace

// CStatusBar::GetPaneText(int) const -- retail RVA 0x1da670 (mfc140u): `this`
// in RCX, the result slot in RDX, nIndex in R8; clones the pane's CStringData
// into the slot and returns the slot.  (An earlier body here took the result
// slot first and `this` second, which is the wrong order for a member
// function; fixed to the retail order.)
// Symbol: ?GetPaneText@CStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__GetPaneText_CStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const CStatusBar* pThis, CString* pRet, int nIndex) {
    if (pRet == nullptr) return pRet;
    new (pRet) CString(pThis->GetPaneText(nIndex));
    return pRet;
}
// Symbol: ?GetPaneInfo@CStatusBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetPaneInfo_CStatusBar__QEBAXHAEAI0AEAH_Z(
    const CStatusBar* pThis, int nIndex, UINT* pnID, UINT* pnStyle, int* pcxWidth) {
    UINT id = 0;
    UINT style = 0;
    int width = 0;
    if (pThis) {
        pThis->GetPaneInfo(nIndex, id, style, width);
    }
    if (pnID) *pnID = id;
    if (pnStyle) *pnStyle = style;
    if (pcxWidth) *pcxWidth = width;
}
// Symbol: ?GetPaneText@CStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CStatusBar* pThis, int nIndex, CString* pStr) {
    pThis->GetPaneText(nIndex, *pStr);
}
// Retail ctor RVA 0x1d9dd0 (mfc140u): CControlBar::CControlBar(); vfptr;
// m_cyTopBorder = 2; m_cyBottomBorder = 0; m_cxLeftBorder = m_cxRightBorder = 0;
// m_nMinHeight = 0.  (CControlBar's ctor already zeroes the other members.)
CStatusBar::CStatusBar() : m_nMinHeight(0) {
    m_cyTopBorder = 2;
    m_cyBottomBorder = 0;
    m_cxLeftBorder = 0;
    m_cxRightBorder = 0;
}
// Retail dtor RVA 0x1d9e10 (mfc140u): vfptr; AllocElements(0, 0) (destroys
// every pane's strText and frees m_pData); CControlBar::~CControlBar().
CStatusBar::~CStatusBar() {
    impl__AllocElements_CStatusBar__QEAAHHH_Z(this, 0, 0);
}
// Retail RVA 0x1d9e40 (mfc140u): `return CreateEx(pParentWnd, 0, dwStyle, nID);`
// through vslot 109, devirtualised to this file's thunk.
BOOL CStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    return impl__CreateEx_CStatusBar__UEAAHPEAVCWnd__KKI_Z(this, pParentWnd, 0, dwStyle, nID) != 0;
}
// Transcribed from retail CStatusBar::SetIndicators, RVA 0x1d9f70 (mfc140u):
//     if (!AllocElements(nIDCount, sizeof(AFX_STATUSPANE) /*0x18*/)) return FALSE;
//     BOOL bResult = TRUE;
//     if (lpIDArray != NULL) {
//         HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);      // IAT SendMessageW
//         CClientDC dcScreen(NULL);                                         // 0x2a3b20
//         HGDIOBJ hOldFont = hFont ? ::SelectObject(dcScreen.m_hDC, hFont) : NULL;
//         AFX_STATUSPANE* pSBP = _GetPanePtr(0);
//         for (int i = 0; i < nIDCount; i++, pSBP++) {
//             pSBP->nID = *lpIDArray++;
//             pSBP->nFlags |= SBPF_UPDATE;
//             if (pSBP->nID != 0) {
//                 HINSTANCE hInst = AfxFindStringResourceHandle(pSBP->nID);   // 0x2aee00
//                 if (hInst == NULL || !pSBP->strText.LoadString(hInst, pSBP->nID))   // 0xdb70
//                     { bResult = FALSE; break; }
//                 SIZE sz; ::GetTextExtentPoint32W(dcScreen.m_hAttribDC, strText, strText.GetLength(), &sz);
//                 pSBP->cxText = sz.cx;
//                 if (!SetPaneText(i, pSBP->strText, FALSE)) { bResult = FALSE; break; }   // 0x1da6b0
//             } else {
//                 pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;        // IAT GetSystemMetrics (signed /4)
//                 if (i == 0) pSBP->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);   // 0x8000100
//             }
//         }
//         if (hOldFont != NULL) ::SelectObject(dcScreen.m_hDC, hOldFont);
//         dcScreen.~CClientDC();                                           // 0x2a3be0
//     }
//     UpdateAllPanes(TRUE, TRUE);                                          // 0x1da250
//     return bResult;
// CClientDC(NULL) reduces to ::GetDC(NULL) with m_hAttribDC == m_hDC, so the
// screen DC is taken directly here.
BOOL CStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount) {
    if (!impl__AllocElements_CStatusBar__QEAAHHH_Z(this, nIDCount, static_cast<int>(sizeof(StatusPane)))) {
        return FALSE;
    }
    BOOL bResult = TRUE;
    if (lpIDArray != nullptr) {
        HFONT hFont = reinterpret_cast<HFONT>(::SendMessageW(m_hWnd, WM_GETFONT, 0, 0));
        HDC hdcScreen = ::GetDC(nullptr);
        HGDIOBJ hOldFont = nullptr;
        if (hFont != nullptr && hdcScreen != nullptr) hOldFont = ::SelectObject(hdcScreen, hFont);
        for (int i = 0; i < nIDCount; ++i) {
            StatusPane* pSBP = PanePtr(this, i);
            pSBP->nID = lpIDArray[i];
            pSBP->nFlags |= kSBPF_UPDATE;
            if (pSBP->nID != 0) {
                HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(pSBP->nID));
                if (hInst == nullptr ||
                    !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
                        &pSBP->strText, hInst, pSBP->nID)) {
                    bResult = FALSE;
                    break;
                }
                SIZE sz = { 0, 0 };
                ::GetTextExtentPoint32W(hdcScreen, pSBP->strText.GetString(), pSBP->strText.GetLength(), &sz);
                pSBP->cxText = sz.cx;
                if (!SetPaneText(i, pSBP->strText.GetString(), FALSE)) {
                    bResult = FALSE;
                    break;
                }
            } else {
                pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;
                if (i == 0) pSBP->nStyle |= (kSBPS_STRETCH | kSBPS_NOBORDERS);
            }
        }
        if (hOldFont != nullptr) ::SelectObject(hdcScreen, hOldFont);
        if (hdcScreen != nullptr) ::ReleaseDC(nullptr, hdcScreen);
    }
    impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(this, TRUE, TRUE);
    return bResult;
}
// Transcribed from retail CStatusBar::SetPaneInfo, RVA 0x1da5c0 (mfc140u):
//     BOOL bChanged = FALSE;
//     AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
//     pSBP->nID = nID;
//     if (pSBP->nStyle != nStyle) {
//         if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH)        // bt $0x1b
//             bChanged = TRUE;
//         else {
//             pSBP->nFlags |= SBPF_UPDATE;
//             pSBP->nStyle = nStyle;
//             SetPaneText(nIndex, pSBP->strText, TRUE);       // 0x1da6b0
//         }
//         pSBP->nStyle = nStyle;
//     }
//     if (cxWidth != pSBP->cxText) { pSBP->cxText = cxWidth; bChanged = TRUE; }
//     if (bChanged) UpdateAllPanes(TRUE, FALSE);              // 0x1da250
// Retail returns void; the BOOL return here is the header's (afxole.h)
// signature and reports whether the index was valid.  The index guard is the
// pre-existing OpenMFC one (retail has only an ASSERT).
BOOL CStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth) {
    if (!PaneIndexValid(this, nIndex)) return FALSE;
    BOOL bChanged = FALSE;
    StatusPane* pSBP = PanePtr(this, nIndex);
    pSBP->nID = nID;
    if (pSBP->nStyle != nStyle) {
        if ((pSBP->nStyle ^ nStyle) & kSBPS_STRETCH) {
            bChanged = TRUE;
        } else {
            pSBP->nFlags |= kSBPF_UPDATE;
            pSBP->nStyle = nStyle;
            SetPaneText(nIndex, pSBP->strText.GetString(), TRUE);
        }
        pSBP->nStyle = nStyle;
    }
    if (cxWidth != pSBP->cxText) {
        pSBP->cxText = cxWidth;
        bChanged = TRUE;
    }
    if (bChanged) impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(this, TRUE, FALSE);
    return TRUE;
}
// Transcribed from retail CStatusBar::GetPaneInfo, RVA 0x1da590 (mfc140u):
//     nID = pane->nID; nStyle = pane->nStyle; cxWidth = pane->cxText;
// (unchecked index in retail; the zero fill on a bad index is the pre-existing
// OpenMFC guard.)
void CStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const {
    if (PaneIndexValid(this, nIndex)) {
        const StatusPane* pSBP = PanePtr(this, nIndex);
        nID = pSBP->nID;
        nStyle = pSBP->nStyle;
        cxWidth = pSBP->cxText;
    } else {
        nID = 0; nStyle = 0; cxWidth = 0;
    }
}
// Transcribed from retail CStatusBar::CommandToIndex, RVA 0x1da470 (mfc140u):
//     if (m_nCount <= 0) return -1;
//     for (int i = 0; i < m_nCount; i++) if (pane[i].nID == nIDFind) return i;
//     return -1;
int CStatusBar::CommandToIndex(UINT nIDFind) const {
    if (m_nCount <= 0 || m_pData == nullptr) return -1;
    for (int i = 0; i < m_nCount; ++i) {
        if (PanePtr(this, i)->nID == nIDFind) return i;
    }
    return -1;
}
// Retail RVA 0x1da4a0 (mfc140u): `return pane[nIndex].nID;` (unchecked; the
// guard is the pre-existing OpenMFC one).
UINT CStatusBar::GetItemID(int nIndex) const {
    if (!PaneIndexValid(this, nIndex)) return 0;
    return PanePtr(this, nIndex)->nID;
}
// Transcribed from retail CStatusBar::GetItemRect, RVA 0x1da4c0 (mfc140u):
//     if (!DefWindowProc(SB_GETRECT, nIndex, (LPARAM)lpRect))   // vslot 73, 0x40a
//         ::SetRectEmpty(lpRect);                                // IAT SetRectEmpty
void CStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (lpRect == nullptr) return;   // retail would fault
    if (!SbDefWindowProc(this, SB_GETRECT, static_cast<WPARAM>(nIndex), reinterpret_cast<LPARAM>(lpRect))) {
        ::SetRectEmpty(lpRect);
    }
}
// Transcribed from retail CStatusBar::SetPaneText, RVA 0x1da6b0 (mfc140u):
//     AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
//     if (!(pSBP->nFlags & SBPF_UPDATE) &&
//         ((lpszNewText == NULL && pSBP->strText.IsEmpty()) ||
//          (lpszNewText != NULL && wcscmp(pSBP->strText, lpszNewText) == 0)))   // IAT wcscmp
//         return TRUE;                                        // nothing to change
//     if (lpszNewText != NULL) pSBP->strText.SetString(lpszNewText, wcslen(lpszNewText));   // 0x2e30
//     else                     pSBP->strText.Empty();                                        // 0x33b0
//     if (!bUpdate) { pSBP->nFlags |= SBPF_UPDATE; return TRUE; }   // can't update now
//     pSBP->nFlags &= ~SBPF_UPDATE;
//     LPCTSTR lpsz = (pSBP->nStyle & SBPS_DISABLED) ? NULL : (LPCTSTR)pSBP->strText;
//     DefWindowProc(SB_SETTEXT /*0x40b*/, ((WORD)pSBP->nStyle) | nIndex, (LPARAM)lpsz);   // vslot 73
//     return TRUE;
// Retail returns TRUE on every path (the `xor %eax,%eax` at 0x1da78c is
// unreachable); the FALSE for a bad index is the pre-existing OpenMFC guard.
BOOL CStatusBar::SetPaneText(int nIndex, const wchar_t* lpszNewText, BOOL bUpdate) {
    if (!PaneIndexValid(this, nIndex)) return FALSE;
    StatusPane* pSBP = PanePtr(this, nIndex);
    if (!(pSBP->nFlags & kSBPF_UPDATE) &&
        ((lpszNewText == nullptr && pSBP->strText.IsEmpty()) ||
         (lpszNewText != nullptr && ::wcscmp(pSBP->strText.GetString(), lpszNewText) == 0))) {
        return TRUE;
    }
    if (lpszNewText != nullptr) {
        // SetIndicators / SetPaneStyle / UpdateAllPanes pass the pane's own
        // buffer here (retail's SetString copes with the overlap); an
        // identical pointer is already the wanted content, so skip the copy.
        if (lpszNewText != pSBP->strText.GetString()) pSBP->strText = lpszNewText;
    } else {
        pSBP->strText.Empty();
    }
    if (!bUpdate) {
        pSBP->nFlags |= kSBPF_UPDATE;
        return TRUE;
    }
    pSBP->nFlags &= ~kSBPF_UPDATE;
    const wchar_t* lpsz = (pSBP->nStyle & kSBPS_DISABLED) ? nullptr : pSBP->strText.GetString();
    SbDefWindowProc(this, SB_SETTEXTW,
                    static_cast<WPARAM>(static_cast<WORD>(pSBP->nStyle)) | static_cast<WPARAM>(static_cast<INT_PTR>(nIndex)),
                    reinterpret_cast<LPARAM>(lpsz));
    return TRUE;
}
// Retail RVA 0x1da670 (mfc140u): `return CString(pane[nIndex].strText);`
// (CloneData on the pane's CStringData; unchecked index -- the empty result
// for a bad index is the pre-existing OpenMFC guard).
CString CStatusBar::GetPaneText(int nIndex) const {
    if (!PaneIndexValid(this, nIndex)) return CString();
    return PanePtr(this, nIndex)->strText;
}
// Retail RVA 0x1da650 (mfc140u): `rString = pane[nIndex].strText;` (tail jump
// to CSimpleStringT::operator=; unchecked index, see above).
void CStatusBar::GetPaneText(int nIndex, CString& rString) const {
    if (!PaneIndexValid(this, nIndex)) { rString.Empty(); return; }
    rString = PanePtr(this, nIndex)->strText;
}
// Transcribed from retail CStatusBar::SetPaneStyle, RVA 0x1da520 (mfc140u):
//     AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
//     if (pSBP->nStyle != nStyle) {
//         if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH) {      // bt $0x1b: re-layout
//             pSBP->nStyle = nStyle;
//             UpdateAllPanes(TRUE, FALSE);                    // 0x1da250
//         }
//         pSBP->nFlags |= SBPF_UPDATE;                        // use SetPaneText, since it
//         pSBP->nStyle = nStyle;                              // updates the style and text
//         SetPaneText(nIndex, pSBP->strText, TRUE);           // 0x1da6b0
//     }
void CStatusBar::SetPaneStyle(int nIndex, UINT nStyle) {
    if (!PaneIndexValid(this, nIndex)) return;   // pre-existing OpenMFC guard
    StatusPane* pSBP = PanePtr(this, nIndex);
    if (pSBP->nStyle != nStyle) {
        if ((pSBP->nStyle ^ nStyle) & kSBPS_STRETCH) {
            pSBP->nStyle = nStyle;
            impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(this, TRUE, FALSE);
        }
        pSBP->nFlags |= kSBPF_UPDATE;
        pSBP->nStyle = nStyle;
        SetPaneText(nIndex, pSBP->strText.GetString(), TRUE);
    }
}
// Retail RVA 0x1da500 (mfc140u): `return pane[nIndex].nStyle;` (unchecked).
UINT CStatusBar::GetPaneStyle(int nIndex) const {
    if (!PaneIndexValid(this, nIndex)) return 0;   // pre-existing OpenMFC guard
    return PanePtr(this, nIndex)->nStyle;
}
CSize CStatusBar::GetPaneSize(int nIndex) const {
    CSize sz(0, 0);
    if (m_hWnd) {
        RECT rc = {};
        GetItemRect(nIndex, &rc);
        sz.cx = rc.right - rc.left;
        sz.cy = rc.bottom - rc.top;
    }
    return sz;
}
CSize CStatusBar::GetBorders() const {
    CSize sz(0, 0);
    if (m_hWnd) {
        int borders[3] = {0, 0, 0};
        if (SbDefWindowProc(this, SB_GETBORDERS, 0, reinterpret_cast<LPARAM>(borders))) {
            sz.cx = borders[0];
            sz.cy = borders[1];
        }
    }
    return sz;
}
void CStatusBar::SetToolTips(CToolTipCtrl* pToolTip) {
    // Real MFC CStatusBar has no tooltip member (status panes manage their own).
    (void)pToolTip;
}
CToolTipCtrl* CStatusBar::GetToolTips() const {
    return nullptr;
}
void CStatusBar::EnableDocking(DWORD dwDockStyle) {
    m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | (dwDockStyle & CBRS_ALIGN_ANY);
}
BOOL CStatusBar::IsSimple() const {
    if (!m_hWnd) return FALSE;
    return (BOOL)SbDefWindowProc(this, SB_ISSIMPLE, 0, 0);
}
// Transcribed from retail CStatusBar::AllocElements, RVA 0x1da0e0 (mfc140u):
//     // destroy old elements
//     for (int i = 0; i < m_nCount; i++)
//         _GetPanePtr(i)->strText.~CString();               // inlined CSimpleStringT dtor: Release() the CStringData
//     // allocate new elements
//     if (!CControlBar::AllocElements(nElements, cbElement))   // 0x1d6070
//         return FALSE;
//     // construct new elements
//     for (int i = 0; i < m_nCount; i++)
//         new (&_GetPanePtr(i)->strText) CString;            // nil string: vslot 3 (GetNilString) of the string manager
//                                                            // object at 0x3b25e8, the one AfxGetStringManager (0x273690) returns
//     return TRUE;
// DEVIATION: core/controlbar/CControlBar.cpp's AllocElements returns FALSE and
// leaves the array alone for nCount <= 0, where retail (0x1d6070) frees the
// old array, stores NULL / 0 and returns TRUE.  The destructor's
// AllocElements(0, 0) depends on that, so the zero case is finished here.
// Symbol: ?AllocElements@CStatusBar@@QEAAHHH@Z
extern "C" int MS_ABI impl__AllocElements_CStatusBar__QEAAHHH_Z(CStatusBar* pThis, int nElements, int cbElement) {
    if (pThis == nullptr) return FALSE;
    if (pThis->m_pData != nullptr) {
        for (int i = 0; i < pThis->m_nCount; ++i) {
            PanePtr(pThis, i)->strText.~CString();
        }
    }
    if (nElements <= 0) {
        if (nElements < 0 || cbElement < 0) {
            impl__AfxThrowInvalidArgException__YAXXZ();   // retail: ENSURE_ARG
            return FALSE;
        }
        free(pThis->m_pData);
        pThis->m_pData = nullptr;
        pThis->m_nCount = 0;
        return TRUE;
    }
    if (!impl__AllocElements_CControlBar__QEAAHHH_Z(pThis, nElements, cbElement)) {
        return FALSE;
    }
    for (int i = 0; i < pThis->m_nCount; ++i) {
        new (&PanePtr(pThis, i)->strText) CString();
    }
    return TRUE;
}

// Transcribed from retail CStatusBar::CalcFixedLayout, RVA 0x1da7b0 (mfc140u).
// `this` in RCX, the CSize result slot in RDX, bStretch in R8 (unused), bHorz
// in R9:
//     TEXTMETRIC tm;
//     {
//         CClientDC dc(NULL);                                            // 0x2a3b20
//         HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);  // IAT SendMessageW
//         HGDIOBJ hOldFont = hFont ? ::SelectObject(dc.m_hDC, hFont) : NULL;
//         ::GetTextMetricsW(dc.m_hAttribDC, &tm);                         // IAT GetTextMetricsW
//         if (hOldFont) ::SelectObject(dc.m_hDC, hOldFont);
//     }                                                                  // 0x2a3be0
//     CRect rectSize; rectSize.SetRectEmpty();                           // IAT SetRectEmpty
//     CalcInsideRect(rectSize, bHorz);                                   // vslot 94 (will be negative size)
//     int rgBorders[3]; DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);   // vslot 73
//     CSize size;
//     size.cx = 32767;
//     size.cy = tm.tmHeight - 1
//             + 2 * (::GetSystemMetrics(SM_CYBORDER) + rgBorders[1])     // IAT GetSystemMetrics, `mov $0x6,%ecx` at 0x1da887
//             - rectSize.Height() - tm.tmInternalLeading;
//     if (size.cy < m_nMinHeight) size.cy = m_nMinHeight;                // +0x148
//     return size;
// Symbol: ?CalcFixedLayout@CStatusBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CStatusBar__UEAA_AVCSize__HH_Z(CStatusBar* pThis, CSize* pRet, int bStretch, int bHorz) {
    (void)bStretch;
    if (pRet == nullptr) return pRet;
    if (pThis == nullptr) { new (pRet) CSize(0, 0); return pRet; }
    TEXTMETRICW tm;
    std::memset(&tm, 0, sizeof(tm));
    {
        HDC hdc = ::GetDC(nullptr);
        HFONT hFont = reinterpret_cast<HFONT>(::SendMessageW(pThis->m_hWnd, WM_GETFONT, 0, 0));
        HGDIOBJ hOldFont = nullptr;
        if (hFont != nullptr && hdc != nullptr) hOldFont = ::SelectObject(hdc, hFont);
        if (hdc != nullptr) ::GetTextMetricsW(hdc, &tm);
        if (hOldFont != nullptr) ::SelectObject(hdc, hOldFont);
        if (hdc != nullptr) ::ReleaseDC(nullptr, hdc);
    }
    CRect rectSize;
    rectSize.SetRectEmpty();
    impl__CalcInsideRect_CStatusBar__UEBAXAEAVCRect__H_Z(pThis, rectSize, bHorz);   // vslot 94
    int rgBorders[3];
    GetSbBorders(pThis, rgBorders);
    int cy = static_cast<int>(tm.tmHeight) - 1
           + 2 * (::GetSystemMetrics(SM_CYBORDER) + rgBorders[1])
           - rectSize.Height() - static_cast<int>(tm.tmInternalLeading);
    if (cy < pThis->m_nMinHeight) cy = pThis->m_nMinHeight;
    new (pRet) CSize(32767, cy);
    return pRet;
}

// Transcribed from retail CStatusBar::CalcInsideRect, RVA 0x1da1a0 (mfc140u):
//     CControlBar::CalcInsideRect(rect, bHorz);                          // 0x1d7c50 (direct call)
//     if ((GetStyle() & SBARS_SIZEGRIP) && !::IsZoomed(::GetParent(m_hWnd))) {   // 0x2a9690; IAT GetParent, IsZoomed
//         // adjust right edge for size grip
//         int rgBorders[3]; DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);   // vslot 73
//         rect.right -= rgBorders[0] + ::GetSystemMetrics(SM_CXVSCROLL)
//                     + ::GetSystemMetrics(SM_CXBORDER) * 2;              // IAT GetSystemMetrics
//     }
// DEVIATION (inherited, not introduced here): retail's CControlBar::CalcInsideRect
// (0x1d7c50, mfc140u) first applies the four CBRS_BORDER_* bits of m_dwStyle
// (bits 8..11 -> afxData.cxBorder2 / cyBorder2) and then picks which of
// m_cxLeftBorder / m_cyTopBorder goes on which edge according to bHorz, plus a
// +7 nudge when (m_dwStyle & 0x400001) == 0x400000.  OpenMFC's thunk
// (core/controlbar/CControlBar.cpp) ignores bHorz and the border bits and just
// insets by the four border members, so every caller of the base here --
// this function, OnNcCalcSize, CalcFixedLayout and UpdateAllPanes -- inherits
// that simplification.  Reported in headerRequests; nothing is worked around
// locally because the correct result is not derivable without afxData.
// Symbol: ?CalcInsideRect@CStatusBar@@UEBAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__CalcInsideRect_CStatusBar__UEBAXAEAVCRect__H_Z(const CStatusBar* pThis, CRect& rect, int bHorz) {
    if (pThis == nullptr) return;
    impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pThis, rect, bHorz);
    if ((impl__GetStyle_CWnd__QEBAKXZ(pThis) & kSBARS_SIZEGRIP) && !::IsZoomed(::GetParent(pThis->m_hWnd))) {
        int rgBorders[3];
        GetSbBorders(pThis, rgBorders);
        rect.right -= rgBorders[0] + ::GetSystemMetrics(SM_CXVSCROLL) + ::GetSystemMetrics(SM_CXBORDER) * 2;
    }
}

// Transcribed from retail CStatusBar::CreateEx, RVA 0x1d9e70 (mfc140u):
//     m_dwStyle = dwStyle & CBRS_ALL;                                    // & 0x40ffff -> +0x124
//     // translate MFC style bits to windows style bits
//     dwStyle &= ~CBRS_ALL;
//     dwStyle |= CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;   // 0x4e
//     if (pParentWnd->GetStyle() & WS_THICKFRAME)                        // 0x2a9690, & 0x40000
//         dwStyle |= SBARS_SIZEGRIP;                                     // 0x100
//     dwStyle |= dwCtrlStyle;
//     AfxEndDeferRegisterClass(AFX_WNDCOMMCTLS_REG /*0x1000*/);          // 0x2918f0 (result ignored)
//     CRect rect; rect.SetRectEmpty();                                   // IAT SetRectEmpty
//     return CWnd::Create(STATUSCLASSNAME /*L"msctls_statusbar32"*/, NULL, dwStyle, rect, pParentWnd, nID, NULL);   // 0x28b690 (direct call)
// Retail reads pParentWnd unguarded (ASSERT_VALID only); a NULL parent fails
// here.  Two notes on OpenMFC's CWnd::Create thunk (core/window/CWnd.cpp):
// retail's CWnd::Create (0x28b690) ORs WS_CHILD into the style before its
// CreateEx (`bts $0x1e,%esi`) and the OpenMFC thunk does not, so WS_CHILD is
// folded in here; and the thunk devirtualises PreCreateWindow to CWnd's, so
// the CStatusBar / CControlBar PreCreateWindow overrides are not reached from
// this path (retail reaches them through vslot 27).
// Symbol: ?CreateEx@CStatusBar@@UEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__CreateEx_CStatusBar__UEAAHPEAVCWnd__KKI_Z(CStatusBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, unsigned int nID) {
    if (pThis == nullptr || pParentWnd == nullptr) return FALSE;
    pThis->m_dwStyle = static_cast<DWORD>(dwStyle) & kCBRS_ALL;
    DWORD dwWinStyle = (static_cast<DWORD>(dwStyle) & ~kCBRS_ALL) | kCCS_STATUSBAR_BITS;
    if (impl__GetStyle_CWnd__QEBAKXZ(pParentWnd) & WS_THICKFRAME) {
        dwWinStyle |= kSBARS_SIZEGRIP;
    }
    dwWinStyle |= static_cast<DWORD>(dwCtrlStyle);
    impl__AfxEndDeferRegisterClass__YAHJ_Z(kAFX_WNDCOMMCTLS_REG);
    RECT rect;
    ::SetRectEmpty(&rect);
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        pThis, STATUSCLASSNAMEW, nullptr, dwWinStyle | WS_CHILD, rect, pParentWnd, nID, nullptr);
}

// Retail: vftable slot 110 of the CStatusBar vftable (0x322438, mfc140u)
// holds 0x27d0 = `ret`, i.e. CStatusBar::DrawItem is an empty body (the
// source's ASSERT(FALSE) compiles away in retail).  This empty body is the
// complete retail behaviour.
// Symbol: ?DrawItem@CStatusBar@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CStatusBar__UEAAXPEAUtagDRAWITEMSTRUCT___Z(CStatusBar* /*pThis*/, DRAWITEMSTRUCT* /*lpDrawItemStruct*/) {
}

// Transcribed from retail CStatusBar::OnBarStyleChange, RVA 0x1da990 (mfc140u):
//     if (m_hWnd != NULL && ((dwOldStyle ^ dwNewStyle) & CBRS_BORDER_ANY /*0xf00*/))
//         SetWindowPos(NULL, 0, 0, 0, 0,
//                      SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);   // 0x33, 0x2a9a60
// Symbol: ?OnBarStyleChange@CStatusBar@@UEAAXKK@Z
extern "C" void MS_ABI impl__OnBarStyleChange_CStatusBar__UEAAXKK_Z(CStatusBar* pThis, unsigned long dwOldStyle, unsigned long dwNewStyle) {
    if (pThis == nullptr || pThis->m_hWnd == nullptr) return;
    if ((dwOldStyle ^ dwNewStyle) & kCBRS_BORDER_ANY) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pThis, nullptr, 0, 0, 0, 0,
                                                    SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }
}

// Transcribed from retail CStatusBar::OnChildNotify, RVA 0x1da9d0 (mfc140u):
//     if (message != WM_DRAWITEM /*0x2b*/)
//         return CWnd::OnChildNotify(message, wParam, lParam, pResult);   // tail jump 0x28f0d0
//     DrawItem((LPDRAWITEMSTRUCT)lParam);                                // vslot 110
//     return TRUE;
// vslot 110 is devirtualised to this file's DrawItem thunk (retail's slot is
// the empty 0x27d0); a client override of DrawItem is therefore not reached.
// Symbol: ?OnChildNotify@CStatusBar@@MEAAHI_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnChildNotify_CStatusBar__MEAAHI_K_JPEA_J_Z(CStatusBar* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam, __int64* pResult) {
    if (pThis == nullptr) return FALSE;
    if (message != WM_DRAWITEM) {
        return impl__OnChildNotify_CWnd__MEAAHI_K_JPEA_J_Z(pThis, message, wParam, lParam, pResult);
    }
    impl__DrawItem_CStatusBar__UEAAXPEAUtagDRAWITEMSTRUCT___Z(pThis, reinterpret_cast<DRAWITEMSTRUCT*>(lParam));
    return TRUE;
}

// Transcribed from retail CStatusBar::OnGetText, RVA 0x1daab0 (mfc140u; WM_GETTEXT
// entry of the message map):
//     if (nMaxChars == 0) return 0;
//     int nIndex = CommandToIndex(0);                       // inlined scan for nID == 0
//     int nLen = 0;
//     if (nIndex >= 0) {
//         nLen = pane[nIndex].strText.GetLength();
//         if (nLen > nMaxChars) nLen = nMaxChars - 1;
//         Checked::memcpy_s(lpszDest, nMaxChars * sizeof(WCHAR), strText, nLen * sizeof(WCHAR));
//             // nLen == 0: skipped; lpszDest == NULL: EINVAL + AfxThrowInvalidArgException
//     }
//     lpszDest[nLen] = 0;
//     return nLen + 1;
// Retail quirk kept as-is: when nLen == nMaxChars the terminator lands at
// lpszDest[nMaxChars].
// Symbol: ?OnGetText@CStatusBar@@IEAAHHPEA_W@Z
extern "C" int MS_ABI impl__OnGetText_CStatusBar__IEAAHHPEA_W_Z(CStatusBar* pThis, int nMaxChars, wchar_t* lpszDest) {
    if (pThis == nullptr || nMaxChars == 0) return 0;
    int nLen = 0;
    const int nIndex = FindMessagePane(pThis);
    if (nIndex >= 0) {
        const StatusPane* pSBP = PanePtr(pThis, nIndex);
        nLen = pSBP->strText.GetLength();
        if (nLen > nMaxChars) nLen = nMaxChars - 1;
        if (nLen > 0) {
            if (lpszDest == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
            std::memcpy(lpszDest, pSBP->strText.GetString(), static_cast<size_t>(nLen) * sizeof(wchar_t));
        }
    }
    if (lpszDest == nullptr) return 0;   // retail would fault on the terminator store
    lpszDest[nLen] = 0;
    return nLen + 1;
}

// Transcribed from retail CStatusBar::OnGetTextLength, RVA 0x1dab90 (mfc140u;
// WM_GETTEXTLENGTH entry):
//     int nIndex = CommandToIndex(0);                       // inlined scan for nID == 0
//     if (nIndex < 0) return 0;
//     return pane[nIndex].strText.GetLength();
// Symbol: ?OnGetTextLength@CStatusBar@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetTextLength_CStatusBar__IEAAIXZ(CStatusBar* pThis) {
    if (pThis == nullptr) return 0;
    const int nIndex = FindMessagePane(pThis);
    if (nIndex < 0) return 0;
    return static_cast<unsigned int>(PanePtr(pThis, nIndex)->strText.GetLength());
}

// Transcribed from retail CStatusBar::OnNcCalcSize, RVA 0x1da910 (mfc140u;
// WM_NCCALCSIZE entry):
//     CRect rect; rect.SetRectEmpty();                      // IAT SetRectEmpty
//     CControlBar::CalcInsideRect(rect, TRUE);              // 0x1d7c50 (direct call, bHorz constant TRUE)
//     lpncsp->rgrc[0].left   += rect.left;
//     lpncsp->rgrc[0].top    += rect.top - 2;
//     lpncsp->rgrc[0].right  += rect.right;
//     lpncsp->rgrc[0].bottom += rect.bottom;
// Symbol: ?OnNcCalcSize@CStatusBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CStatusBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(CStatusBar* pThis, int /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp) {
    if (pThis == nullptr || lpncsp == nullptr) return;
    CRect rect;
    rect.SetRectEmpty();
    impl__CalcInsideRect_CControlBar__UEBAXAEAVCRect__H_Z(pThis, rect, TRUE);
    lpncsp->rgrc[0].left   += rect.left;
    lpncsp->rgrc[0].top    += rect.top - 2;
    lpncsp->rgrc[0].right  += rect.right;
    lpncsp->rgrc[0].bottom += rect.bottom;
}

// Transcribed from retail CStatusBar::OnNcHitTest, RVA 0x1da8f0 (mfc140u;
// WM_NCHITTEST entry; the CPoint argument is unused):
//     LRESULT nResult = CWnd::Default();                    // 0x28ac80
//     return nResult == HTBOTTOMRIGHT ? HTBOTTOMRIGHT : HTCLIENT;   // 0x11 : 1 (cmovne)
// Symbol: ?OnNcHitTest@CStatusBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CStatusBar__IEAA_JVCPoint___Z(CStatusBar* pThis, long long /*point*/) {
    if (pThis == nullptr) return HTCLIENT;
    const __int64 nResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    return nResult == HTBOTTOMRIGHT ? HTBOTTOMRIGHT : HTCLIENT;
}

// Retail CStatusBar::OnNcPaint, RVA 0x1d9430 (mfc140u; WM_NCPAINT entry):
//     EraseNonClient();                                      // tail jump 0x1d6e50
// Symbol: ?OnNcPaint@CStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CStatusBar__IEAAXXZ(CStatusBar* pThis) {
    if (pThis == nullptr) return;
    impl__EraseNonClient_CControlBar__QEAAXXZ(pThis);
}

// Transcribed from retail CStatusBar::OnPaint, RVA 0x1daa00 (mfc140u; WM_PAINT entry):
//     UpdateAllPanes(FALSE, TRUE);                          // 0x1da250
//     CWnd::Default();                                       // tail jump 0x28ac80
// Symbol: ?OnPaint@CStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CStatusBar__IEAAXXZ(CStatusBar* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(pThis, FALSE, TRUE);
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Transcribed from retail CStatusBar::OnSetMinHeight, RVA 0x1dabd0 (mfc140u;
// SB_SETMINHEIGHT (0x408) entry):
//     LRESULT lResult = CWnd::Default();                    // 0x28ac80
//     m_nMinHeight = (int)wParam;                            // +0x148
//     return lResult;
// Symbol: ?OnSetMinHeight@CStatusBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnSetMinHeight_CStatusBar__IEAA_J_K_J_Z(CStatusBar* pThis, unsigned __int64 wParam, __int64 /*lParam*/) {
    if (pThis == nullptr) return 0;
    const __int64 lResult = impl__Default_CWnd__IEAA_JXZ(pThis);
    pThis->m_nMinHeight = static_cast<int>(wParam);
    return lResult;
}

// Transcribed from retail CStatusBar::OnSetText, RVA 0x1daa60 (mfc140u;
// WM_SETTEXT entry):
//     int nIndex = CommandToIndex(0);                       // inlined scan for nID == 0
//     if (nIndex < 0) return -1;
//     return SetPaneText(nIndex, lpszText, TRUE) ? 0 : -1;   // 0x1da6b0
// Symbol: ?OnSetText@CStatusBar@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CStatusBar__IEAAHPEB_W_Z(CStatusBar* pThis, const wchar_t* lpszText) {
    if (pThis == nullptr) return -1;
    const int nIndex = FindMessagePane(pThis);
    if (nIndex < 0) return -1;
    return pThis->SetPaneText(nIndex, lpszText, TRUE) ? 0 : -1;
}

// Transcribed from retail CStatusBar::OnSize, RVA 0x1daa30 (mfc140u; WM_SIZE entry):
//     CWnd::Default();                                       // 0x28ac80
//     UpdateAllPanes(TRUE, FALSE);                           // tail jump 0x1da250
// Symbol: ?OnSize@CStatusBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CStatusBar__IEAAXIHH_Z(CStatusBar* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(pThis, TRUE, FALSE);
}

// Transcribed from retail CStatusBar::OnUpdateCmdUI, RVA 0x1daca0 (mfc140u):
//     CStatusCmdUI state;                                    // vftable 0x322298; the CCmdUI ctor's memset is inlined
//     state.m_pOther = this;
//     state.m_nIndexMax = (UINT)m_nCount;
//     for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++) {
//         state.m_nID = _GetPanePtr(state.m_nIndex)->nID;
//         // allow the statusbar itself to have update handlers
//         if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))   // 0x1de460 = CCmdTarget::OnCmdMsg, direct call
//             continue;
//         // allow target (owner) to handle the remaining updates -- CCmdUI::DoUpdate(pTarget, FALSE) inlined:
//         if (state.m_nID == 0 || LOWORD(state.m_nID) == 0xFFFF) continue;
//         ENSURE_ARG(pTarget != NULL);                       // AfxThrowInvalidArgException
//         state.m_bEnableChanged = FALSE;
//         pTarget->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL);   // vslot 5
//     }
//     // update the dialog controls added to the status bar
//     UpdateDialogControls(pTarget, bDisableIfNoHndler);     // 0x291460
// The status bar's own CCmdTarget::OnCmdMsg is called through its thunk, which
// in this tree (core/runtime/CCmdTarget.cpp) is a stub returning 0, so the
// "statusbar itself handles it" branch never fires until that is implemented.
// Symbol: ?OnUpdateCmdUI@CStatusBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CStatusBar__UEAAXPEAVCFrameWnd__H_Z(CStatusBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;
    MsCmdUI state;
    std::memset(&state, 0, sizeof(state));
    state.vfptr = g_msCmdUIVtbl;
    state.m_pOther = pThis;
    state.m_nIndexMax = static_cast<UINT>(pThis->m_nCount);
    for (state.m_nIndex = 0; static_cast<UINT>(state.m_nIndex) < state.m_nIndexMax; ++state.m_nIndex) {
        if (pThis->m_pData == nullptr) break;   // retail would fault
        state.m_nID = PanePtr(pThis, state.m_nIndex)->nID;
        if (impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(pThis, state.m_nID, kCN_UPDATE_COMMAND_UI, &state, nullptr)) {
            continue;
        }
        if (state.m_nID == 0 || static_cast<WORD>(state.m_nID) == 0xFFFF) continue;
        if (pTarget == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        state.m_bEnableChanged = FALSE;
        void** vtbl = *reinterpret_cast<void***>(pTarget);
        OnCmdMsgFn fnOnCmdMsg = reinterpret_cast<OnCmdMsgFn>(vtbl[kVtOnCmdMsgSlot]);
        fnOnCmdMsg(pTarget, state.m_nID, kCN_UPDATE_COMMAND_UI, &state, nullptr);
    }
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisableIfNoHndler);
}

// Transcribed from retail CStatusBar::OnWindowPosChanging, RVA 0x1d94e0 (mfc140u;
// WM_WINDOWPOSCHANGING entry):
//     DWORD dwStyle = m_dwStyle;                             // +0x124
//     m_dwStyle &= ~CBRS_BORDER_ANY;                         // & 0xfffff0ff
//     CControlBar::OnWindowPosChanging(lpWndPos);            // 0x1d69c0 (direct call)
//     m_dwStyle = dwStyle;
// Symbol: ?OnWindowPosChanging@CStatusBar@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CStatusBar__IEAAXPEAUtagWINDOWPOS___Z(CStatusBar* pThis, WINDOWPOS* lpWndPos) {
    if (pThis == nullptr) return;
    const DWORD dwStyle = pThis->m_dwStyle;
    pThis->m_dwStyle &= ~kCBRS_BORDER_ANY;
    impl__OnWindowPosChanging_CControlBar__QEAAXPEAUtagWINDOWPOS___Z(pThis, lpWndPos);
    pThis->m_dwStyle = dwStyle;
}

// Transcribed from retail CStatusBar::PreCreateWindow, RVA 0x1d9f40 (mfc140u;
// vslot 27):
//     if ((m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) == CBRS_BOTTOM)   // & 0xff00 == 0x8200
//         m_dwStyle &= ~(CBRS_BORDER_ANY | CBRS_BORDER_3D);                  // & 0xfffff07f
//     return CControlBar::PreCreateWindow(cs);                              // tail jump 0x1d5fa0
// Symbol: ?PreCreateWindow@CStatusBar@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CStatusBar__UEAAHAEAUtagCREATESTRUCTW___Z(CStatusBar* pThis, CREATESTRUCTW* cs) {
    if (pThis == nullptr || cs == nullptr) return FALSE;
    if ((pThis->m_dwStyle & (kCBRS_ALIGN_ANY | kCBRS_BORDER_ANY)) == kCBRS_BOTTOM) {
        pThis->m_dwStyle &= ~(kCBRS_BORDER_ANY | kCBRS_BORDER_3D);
    }
    return impl__PreCreateWindow_CControlBar__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, *cs);
}

// Transcribed from retail CStatusBar::UpdateAllPanes, RVA 0x1da250 (mfc140u):
//     if (bUpdateRects) {
//         // get border information and client work area
//         CRect rect; ::GetWindowRect(m_hWnd, &rect);                         // IAT GetWindowRect
//         ::OffsetRect(&rect, -rect.left, -rect.top);                         // IAT OffsetRect
//         CalcInsideRect(rect, TRUE);                                         // vslot 94
//         int rgBorders[3]; DefWindowProc(SB_GETBORDERS, 0, (LPARAM)rgBorders);   // vslot 73
//         // determine extra space for stretchy pane
//         int cxExtra = rect.Width() + rgBorders[2];
//         int nStretchyCount = 0;
//         for (int i = 0; i < m_nCount; i++) {
//             AFX_STATUSPANE* pSBP = _GetPanePtr(i);
//             if (pSBP->nStyle & SBPS_STRETCH) ++nStretchyCount;               // testl $0x8000000
//             cxExtra -= (pSBP->cxText + CX_PANE_BORDER + rgBorders[2]);
//         }
//         // determine right edge of each pane
//         CArray<int,int> rightEdges; rightEdges.SetSize(m_nCount);           // vftable 0x322238; 0x15034
//         int right = rgBorders[0];
//         for (int i = 0; i < m_nCount; i++) {
//             AFX_STATUSPANE* pSBP = _GetPanePtr(i);
//             right += pSBP->cxText + CX_PANE_BORDER;
//             if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0) {
//                 int cxAdd = cxExtra / nStretchyCount;                        // idiv
//                 right += cxAdd; cxExtra -= cxAdd; --nStretchyCount;
//             }
//             rightEdges[i] = right;                                          // range-checked: AfxThrowInvalidArgException
//             right += rgBorders[2];
//         }
//         DefWindowProc(SB_SETPARTS /*0x404*/, m_nCount, (LPARAM)rightEdges.GetData());   // vslot 73
//     }                                                                       // ~CArray 0x1dada8
//     // update text in the status panes
//     if (bUpdateText) {
//         for (int i = 0; i < m_nCount; i++) {
//             AFX_STATUSPANE* pSBP = _GetPanePtr(i);
//             if (pSBP->nFlags & SBPF_UPDATE)                                 // testb $0x1,0xc
//                 SetPaneText(i, pSBP->strText, TRUE);                        // 0x1da6b0
//         }
//     }
// Symbol: ?UpdateAllPanes@CStatusBar@@IEAAXHH@Z
extern "C" void MS_ABI impl__UpdateAllPanes_CStatusBar__IEAAXHH_Z(CStatusBar* pThis, int bUpdateRects, int bUpdateText) {
    if (pThis == nullptr) return;
    if (bUpdateRects) {
        RECT rect = { 0, 0, 0, 0 };
        ::GetWindowRect(pThis->m_hWnd, &rect);
        ::OffsetRect(&rect, -rect.left, -rect.top);
        CRect rectInside(rect);
        impl__CalcInsideRect_CStatusBar__UEBAXAEAVCRect__H_Z(pThis, rectInside, TRUE);   // vslot 94
        int rgBorders[3];
        GetSbBorders(pThis, rgBorders);
        int cxExtra = rectInside.Width() + rgBorders[2];
        int nStretchyCount = 0;
        const int nCount = pThis->m_pData != nullptr ? pThis->m_nCount : 0;
        for (int i = 0; i < nCount; ++i) {
            const StatusPane* pSBP = PanePtr(pThis, i);
            if (pSBP->nStyle & kSBPS_STRETCH) ++nStretchyCount;
            cxExtra -= (pSBP->cxText + kCX_PANE_BORDER + rgBorders[2]);
        }
        std::vector<int> rightEdges(static_cast<size_t>(nCount > 0 ? nCount : 0));
        int right = rgBorders[0];
        for (int i = 0; i < nCount; ++i) {
            const StatusPane* pSBP = PanePtr(pThis, i);
            right += pSBP->cxText + kCX_PANE_BORDER;
            if ((pSBP->nStyle & kSBPS_STRETCH) && cxExtra > 0) {
                const int cxAdd = cxExtra / nStretchyCount;
                right += cxAdd;
                cxExtra -= cxAdd;
                --nStretchyCount;
            }
            rightEdges[static_cast<size_t>(i)] = right;
            right += rgBorders[2];
        }
        SbDefWindowProc(pThis, SB_SETPARTS, static_cast<WPARAM>(nCount),
                        reinterpret_cast<LPARAM>(rightEdges.empty() ? nullptr : rightEdges.data()));
    }
    if (bUpdateText && pThis->m_pData != nullptr) {
        for (int i = 0; i < pThis->m_nCount; ++i) {
            StatusPane* pSBP = PanePtr(pThis, i);
            if (pSBP->nFlags & kSBPF_UPDATE) {
                pThis->SetPaneText(i, pSBP->strText.GetString(), TRUE);
            }
        }
    }
}
