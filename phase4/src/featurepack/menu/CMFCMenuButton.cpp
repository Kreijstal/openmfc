// CMFCMenuButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// How this file was written
// ---------------------------------------------------------------------------
// Every body below was transcribed from the retail disassembly (the method in
// the header comment of core/ole/COleControl.cpp).  Each body names its entry
// RVA in mfc140u.dll, found through the export ordinal table; each one was
// also compared with the mfc140.dll (ANSI twin) body and the instruction
// sequences are identical apart from absolute/RIP-relative addresses.  Every IAT slot named below was resolved with
// iatu.py against mfc140u, and every callee RVA with uwho.py (mfc140u).
//
// ---------------------------------------------------------------------------
// Layout
// ---------------------------------------------------------------------------
// OpenMFC declares no `class CMFCMenuButton` in include/openmfc/ and there is
// no detail/*Support.h shadow for it, so this file pins the retail layout
// itself (S_MenuButton below).  The derived members and their order come from
// the shipping afxmenubutton.h:31 (read on this host); their offsets come from
// the constructor ??0CMFCMenuButton@@QEAA@XZ (entry RVA 0x8e2c0, mfc140u):
//     call ??0CMFCButton@@QEAA@XZ                   (0x1c930)
//     vfptr = CMFCMenuButton vftable                (0x1802efab8, mfc140u)
//     +0xb38 = 1                                    m_bOSMenu
//     +0xb20 = 0 (4 bytes)                          m_bRightArrow
//     +0xb28 = 0 (8 bytes)                          m_hMenu
//     +0xb30 = 0 (8 bytes)                          m_nMenuResult, m_bStayPressed
//     +0xb3c = 0 (8 bytes)                          m_bDefaultClick, m_bMenuIsActive
//     +0xb44 = 0 (4 bytes)                          m_bClickOnMenu
// sizeof == 0xb48 (2888), the size the CRuntimeClass descriptor in
// featurepack/menu/RuntimeClasses.cpp records.
//
// The CMFCButton members the retail bodies touch are named from the shipping
// afxbutton.h (member order counted from CButton's 0xe8 bytes) and confirmed
// by the retail bodies: CMFCButton::OnLButtonDown (entry 0x1e010, mfc140u)
// tests +0x108 (m_bResponseOnButtonDown), sets +0x114/+0x118/+0x11c
// (m_bPushed / m_bClickiedInside / m_bHighlighted), sets +0x120 (m_bCaptured)
// after ::SetCapture, and reads +0x144 (m_nAutoRepeatTimeDelay) before
// ::SetTimer(0xec0d).  NOTE: the names in detail/CMFCButtonSupport.h
// (S_Cmfcbutton) for +0x10c..+0x124 do NOT match these (it calls +0x10c
// m_bTransparent, +0x110 m_bPushed and +0x114 m_bHover); only the offsets
// below are relied on here.
//
// ---------------------------------------------------------------------------
// vptr and virtual calls
// ---------------------------------------------------------------------------
// ??0CMFCButton@@QEAA@XZ (featurepack/controls/Thunks.cpp) placement-news
// OpenMFC's C++ CMFCButton, so an object built here carries OpenMFC's mingw
// CMFCButton vtable; there is no MSVC-layout CMFCMenuButton vtable in this
// DLL.  The retail bodies make two kinds of virtual call on `this`, through
// the CMFCMenuButton vftable (mfc140.dll twin 0x1802eda08, dumped slot by
// slot with vtdump.py):
//   +0x2f0 slot  94  SizeToContent   (== ?SizeToContent@CMFCMenuButton@@)
//   +0x340 slot 104  OnShowMenu      (== ?OnShowMenu@CMFCMenuButton@@)
// Those slot numbers mean something else on OpenMFC's mingw table, so each
// site goes through a Dispatch* helper, the same scheme as
// featurepack/controls/CMFCEditBrowseCtrl.cpp: while the object still carries
// the vptr this file's constructor recorded (g_ownVptr), this file's own
// thunk is called directly; when the vptr is anything else -- a client class
// compiled against the real headers installs its own MSVC vftable after our
// constructor returns -- the call goes through that table's slot exactly as
// retail does, so a client override IS reached.  The destructor stores
// g_ownVptr back first (retail's "vfptr = &vftable" store) so that
// ??1CMFCButton@@'s C++ virtual-destructor call dispatches on OpenMFC's table.
//
// Message routing.  The retail message map (0x1802ef9a0 in mfc140u, dumped
// with msgmap_u.py; mfc140.dll twin 0x1802ed8f0, dumped with msgmap_a.py) routes WM_KEYDOWN, WM_LBUTTONDOWN, WM_GETDLGCODE,
// WM_LBUTTONUP, WM_KILLFOCUS, WM_LBUTTONDBLCLK and WM_MFC_INITCTRL (0x37c) to
// the handlers below; OpenMFC's classCMFCMenuButton_msgmap
// (detail/Mfc05MsgmapSupport.cpp) currently has no entries, so these handlers
// are reachable only as exports until that map is filled in.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

// ---------------------------------------------------------------------------
// Thunks this file calls (definitions named on each line).
// ---------------------------------------------------------------------------
// featurepack/controls/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCButton__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCButton__UEAA_XZ(void* pThis);
// featurepack/controls/CMFCButton.cpp
extern "C" void  MS_ABI impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState);
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly);
extern "C" void  MS_ABI impl__OnLButtonDown_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point);
extern "C" void  MS_ABI impl__OnLButtonUp_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point);
extern "C" void  MS_ABI impl__OnLButtonDblClk_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point);
extern "C" void  MS_ABI impl__OnKillFocus_CMFCButton__IEAAXPEAVCWnd___Z(
    void* pThis, void* pNewWnd);
extern "C" int   MS_ABI impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg);
// featurepack/controls/StaticData.cpp : ?m_bWinXPTheme@CMFCButton@@1HA (0x3be1a8, mfc140u)
extern "C" std::int32_t impl__m_bWinXPTheme_CMFCButton__1HA;
// core/runtime/StaticData.cpp : ?m_pActivePopupMenu@CMFCPopupMenu@@1PEAV1@EA (0x3be288, mfc140u)
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;
// core/window/Thunks.cpp, core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void*   MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int     MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" CWnd*   MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int     MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
// featurepack/controls/CMFCReBar.cpp
extern "C" CSize* MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(CSize* pRet);
extern "C" void   MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC* pDC, int id, const CRect* rectImage, int state, const CSize* sizeImage);
// core/gdi/CDC.cpp
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
// core/runtime/AFX_GLOBAL_DATA.cpp, featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];   // 0x3c1620 (mfc140u)
// featurepack/controls/CMFCControlContainer.cpp
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8);
// featurepack/controls/CTagManager.cpp, featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* self);
extern "C" int  MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(
    void* tagManager, const wchar_t* lpszProp, int* pbValue);
// detail/RegcoreSupport.cpp, core/runtime/CObject.cpp, core/app/CWinAppEx.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
// featurepack/menu/CContextMenuManager.cpp
extern "C" unsigned int MS_ABI impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(
    CContextMenuManager* pThis, HMENU hMenu, int x, int y, CWnd* pWndOwner, int bRightAlign);

// Forward declarations of this file's own exports (used by the Dispatch helpers).
extern "C" void   MS_ABI impl__OnShowMenu_CMFCMenuButton__MEAAXXZ(void* pThis);
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCMenuButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly);

namespace {

// ---------------------------------------------------------------------------
// Retail CMFCMenuButton layout (see the file header).
// ---------------------------------------------------------------------------
struct S_MenuButton {
    void*  vfptr;                         // 0x000
    char   _pad008[0x040 - 0x008];
    HWND   m_hWnd;                        // 0x040 CWnd::m_hWnd
    char   _pad048[0x10c - 0x048];
    BOOL   m_bDontUseWinXPTheme;          // 0x10c CMFCButton
    BOOL   m_bWasDblClk;                  // 0x110 CMFCButton (not touched here)
    BOOL   m_bPushed;                     // 0x114 CMFCButton
    BOOL   m_bClickiedInside;             // 0x118 CMFCButton
    BOOL   m_bHighlighted;                // 0x11c CMFCButton
    BOOL   m_bCaptured;                   // 0x120 CMFCButton
    BOOL   m_bHover;                      // 0x124 CMFCButton
    char   _pad128[0x150 - 0x128];
    SIZE   m_sizePushOffset;              // 0x150 CMFCButton
    char   _pad158[0xb20 - 0x158];
    BOOL   m_bRightArrow;                 // 0xb20
    HMENU  m_hMenu;                       // 0xb28
    int    m_nMenuResult;                 // 0xb30
    BOOL   m_bStayPressed;                // 0xb34
    BOOL   m_bOSMenu;                     // 0xb38
    BOOL   m_bDefaultClick;               // 0xb3c
    BOOL   m_bMenuIsActive;               // 0xb40 (protected)
    BOOL   m_bClickOnMenu;                // 0xb44 (protected)
};
static_assert(offsetof(S_MenuButton, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(S_MenuButton, m_bDontUseWinXPTheme) == 0x10c, "CMFCButton::m_bDontUseWinXPTheme (OnDraw 0x8e380)");
static_assert(offsetof(S_MenuButton, m_bPushed) == 0x114, "CMFCButton::m_bPushed (CMFCButton::OnLButtonDown 0x1e010)");
static_assert(offsetof(S_MenuButton, m_bClickiedInside) == 0x118, "CMFCButton::m_bClickiedInside");
static_assert(offsetof(S_MenuButton, m_bHighlighted) == 0x11c, "CMFCButton::m_bHighlighted");
static_assert(offsetof(S_MenuButton, m_bCaptured) == 0x120, "CMFCButton::m_bCaptured");
static_assert(offsetof(S_MenuButton, m_bHover) == 0x124, "CMFCButton::m_bHover");
static_assert(offsetof(S_MenuButton, m_sizePushOffset) == 0x150, "CMFCButton::m_sizePushOffset");
static_assert(offsetof(S_MenuButton, m_bRightArrow) == 0xb20, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_hMenu) == 0xb28, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_nMenuResult) == 0xb30, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_bStayPressed) == 0xb34, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_bOSMenu) == 0xb38, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_bDefaultClick) == 0xb3c, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_bMenuIsActive) == 0xb40, "ctor 0x8e2c0 (mfc140u)");
static_assert(offsetof(S_MenuButton, m_bClickOnMenu) == 0xb44, "ctor 0x8e2c0 (mfc140u)");
static_assert(sizeof(S_MenuButton) == 0xb48, "retail sizeof(CMFCMenuButton) == 2888");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "OpenMFC CWnd::m_hWnd must sit where the retail bodies read it");
static_assert(sizeof(CMFCButton) <= 0xb20, "the ??0CMFCButton thunk placement-constructs OpenMFC's CMFCButton inside the retail 0xb20-byte base");

inline S_MenuButton* M(void* p) { return static_cast<S_MenuButton*>(p); }
inline CWnd*         W(void* p) { return static_cast<CWnd*>(p); }

// CPoint passed by value is an 8-byte aggregate in a register under MS_ABI.
inline POINT PtFrom(long long v) { POINT pt; std::memcpy(&pt, &v, sizeof pt); return pt; }

// Retail vftable slots of CMFCMenuButton (see the file header).
constexpr int kSlotSizeToContent = 94;    // +0x2f0
constexpr int kSlotOnShowMenu    = 104;   // +0x340

// CMenuImages enum values (shipping afxmenuimages.h).
constexpr int kIdArrowDownLarge  = 13;    // CMenuImages::IdArrowDownLarge
constexpr int kIdArrowRightLarge = 14;    // CMenuImages::IdArrowRightLarge
constexpr int kImageBlack        = 0;     // CMenuImages::ImageBlack
constexpr int kImageGray         = 1;     // CMenuImages::ImageGray

// AFX_GLOBAL_DATA offsets (shipping afxglobals.h order; the same layout
// core/runtime/AFX_GLOBAL_DATA.cpp transcribes -- it static_asserts 0x000 and
// 0x030, and places clrBtnDkShadow at 0x03c without an assert).  Retail OnDraw
// reads afxGlobalData (0x3c1620, mfc140u) + 0x30 and + 0x3c.
constexpr int kGlobalDataInitGate      = 0x000;   // m_bInitialized
constexpr int kGlobalDataClrBtnHilite  = 0x030;   // clrBtnHilite
constexpr int kGlobalDataClrBtnDkShadow = 0x03c;  // clrBtnDkShadow

// ---------------------------------------------------------------------------
// Virtual dispatch on `this` (see the file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CMFCButton@@QEAA@XZ installs, recorded by the first ctor to run
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *static_cast<void* const*>(p) == g_ownVptr;
}
inline void* SlotOf(void* p, int slot) { return (*static_cast<void***>(p))[slot]; }

inline void DispatchOnShowMenu(void* p) {
    if (HasOwnVptr(p)) { impl__OnShowMenu_CMFCMenuButton__MEAAXXZ(p); return; }
    using Fn = void (MS_ABI*)(void*);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnShowMenu))(p);
}
inline void DispatchSizeToContent(void* p, CSize* pRet, int bCalcOnly) {
    if (HasOwnVptr(p)) { impl__SizeToContent_CMFCMenuButton__UEAA_AVCSize__H_Z(p, pRet, bCalcOnly); return; }
    using Fn = CSize* (MS_ABI*)(void*, CSize*, int);
    reinterpret_cast<Fn>(SlotOf(p, kSlotSizeToContent))(p, pRet, bCalcOnly);
}

// Retail's inline GetGlobalData(): if (!m_bInitialized) { Initialize(); m_bInitialized = TRUE; }
inline void EnsureGlobalDataInitialized() {
    int bInit = 0;
    std::memcpy(&bInit, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof bInit);
    if (bInit == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline COLORREF GlobalColor(int off) {
    COLORREF clr = 0;
    std::memcpy(&clr, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof clr);
    return clr;
}

// Retail's afxContextMenuManager (mfc140u .data 0x3be1b0; not exported), which
// CWinAppEx::InitContextMenuManager fills in (its body, entry RVA 0x1c7830
// (mfc140u), stores the newly constructed manager into 0x3be1b0; the
// mfc140.dll twin's body is at 0x1c5e10 and its global at 0x3b6f10).  OpenMFC publishes no
// such global; the closest reachable equivalent is the manager the current
// CWinAppEx owns (CWinAppEx::m_pContextMenuManager, written by OpenMFC's
// InitContextMenuManager), read the same way core/frame/CMDIFrameWndEx.cpp
// does.  It is read directly rather than through GetContextMenuManager, which
// would create one on demand and turn retail's "none exists" gate into "always".
struct WinAppExAccess : CWinAppEx {
    using CWinAppEx::m_pContextMenuManager;
};
CContextMenuManager* CurrentContextMenuManager() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<WinAppExAccess*>(static_cast<CWinAppEx*>(pApp))->m_pContextMenuManager;
}

// ::ReleaseCapture() + m_bCaptured = FALSE when captured -- the tail every
// mouse/focus handler below shares (ReleaseCapture is import 0x2c7258, mfc140u).
inline void ReleaseCaptureIfCaptured(S_MenuButton* s) {
    if (s->m_bCaptured) {
        ::ReleaseCapture();
        s->m_bCaptured = FALSE;
    }
}

} // namespace

// Retail (entry RVA 0x8e2c0, mfc140u), fully transcribed -- see the file
// header for the member stores.  The vptr store of the CMFCMenuButton vftable
// (0x1802efab8, mfc140u) is not reproduced: OpenMFC has no MSVC-layout table
// for this class, so the object keeps the vptr ??0CMFCButton@@ installed and
// that vptr is recorded as g_ownVptr (file header).
// Symbol: ??0CMFCMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMenuButton__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCButton__QEAA_XZ(pThis);                     // 0x1c930
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    S_MenuButton* s = M(pThis);
    s->m_bOSMenu = TRUE;
    s->m_bRightArrow = FALSE;
    s->m_hMenu = nullptr;
    s->m_nMenuResult = 0;
    s->m_bStayPressed = FALSE;
    s->m_bDefaultClick = FALSE;
    s->m_bMenuIsActive = FALSE;
    s->m_bClickOnMenu = FALSE;
    return pThis;
}

// Retail (entry RVA 0x8e360, mfc140u), fully transcribed:
//     vfptr = &CMFCMenuButton::`vftable';      // 0x1802efab8 -> g_ownVptr here (file header)
//     jmp ??1CMFCButton@@UEAA@XZ               // 0x1cad0
// CMFCMenuButton owns no resources of its own: m_hMenu is not destroyed.
// Symbol: ??1CMFCMenuButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCMenuButton__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CMFCButton__UEAA_XZ(pThis);
}

// Retail (entry RVA 0x8e380, mfc140u), fully transcribed:
//     CSize sizeArrow = CMenuImages::Size();                                // 0x8f600
//     CRect rectParent = rect;  rectParent.right += -10 - sizeArrow.cx;
//     CMFCButton::OnDraw(pDC, rectParent, uiState);                         // 0x1cf60
//     CRect rectArrow = rect;   rectArrow.left = rectParent.right;
//     CMenuImages::Draw(pDC, m_bRightArrow ? IdArrowRightLarge (14) : IdArrowDownLarge (13),
//                       rectArrow, (uiState & ODS_DISABLED) ? ImageGray : ImageBlack,
//                       CSize(0, 0));                                       // 0x8f790
//     if (m_bDefaultClick) {
//         CRect rectSeparator = rectArrow;  rectSeparator.right = rectArrow.left + 2;
//         ::InflateRect(rectSeparator, 0, -2);                             // import 0x2c72e8
//         if (!CMFCButton::m_bWinXPTheme || m_bDontUseWinXPTheme) {         // 0x3be1a8, +0x10c
//             rectSeparator.left += m_sizePushOffset.cx;                    // +0x150
//             rectSeparator.top  += m_sizePushOffset.cy;                    // +0x154
//         }
//         <GetGlobalData() init gate>                                        // Initialize 0x6a790
//         pDC->Draw3dRect(&rectSeparator, afxGlobalData.clrBtnDkShadow,     // +0x3c
//                         afxGlobalData.clrBtnHilite);                       // +0x30; 0x2a5bc0
//     }
// Only left/top of the separator are shifted by the push offset (right and
// bottom are not), exactly as retail does.
// Symbol: ?OnDraw@CMFCMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI impl__OnDraw_CMFCMenuButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, CDC* pDC, const CRect* rect, unsigned int uiState) {
    if (pThis == nullptr || rect == nullptr) return;
    S_MenuButton* s = M(pThis);

    CSize sizeArrow;
    impl__Size_CMenuImages__SA_AVCSize__XZ(&sizeArrow);
    CRect rectParent = *rect;
    rectParent.right += -10 - sizeArrow.cx;
    impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(pThis, pDC, rectParent, uiState);

    CRect rectArrow = *rect;
    rectArrow.left = rectParent.right;
    const CSize sizeZero(0, 0);
    impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
        pDC, s->m_bRightArrow ? kIdArrowRightLarge : kIdArrowDownLarge, &rectArrow,
        (uiState & ODS_DISABLED) ? kImageGray : kImageBlack, &sizeZero);

    if (s->m_bDefaultClick) {
        CRect rectSeparator = rectArrow;
        rectSeparator.right = rectArrow.left + 2;
        ::InflateRect(rectSeparator, 0, -2);
        if (impl__m_bWinXPTheme_CMFCButton__1HA == 0 || s->m_bDontUseWinXPTheme) {
            rectSeparator.left += s->m_sizePushOffset.cx;
            rectSeparator.top += s->m_sizePushOffset.cy;
        }
        EnsureGlobalDataInitialized();
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
            pDC, rectSeparator,
            GlobalColor(kGlobalDataClrBtnDkShadow), GlobalColor(kGlobalDataClrBtnHilite));
    }
}

// Retail: export ordinal 9995 (mfc140u) resolves to RVA 0x3a60, whose body is
//     mov $0x1,%eax ; ret
// -- an identical-code-folded body that the RVA map names after another
// export, which is why disas.py does not find this symbol by name.  The
// mfc140u message map (0x1802ef9a0, the address GetMessageMap 0x8e370 returns;
// dumped with msgmap_u.py) routes WM_GETDLGCODE (0x87, sig 50) to that same
// 0x3a60 body; the mfc140.dll twin's map routes it to its own folded
// `return 1` body (0x3ae0 there).  1 == DLGC_WANTARROWS.
// Symbol: ?OnGetDlgCode@CMFCMenuButton@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCMenuButton__IEAAIXZ(void* pThis) {
    (void)pThis;
    return DLGC_WANTARROWS;
}

// Retail (entry RVA 0x8e930, mfc140u), fully transcribed:
//     CString strDst;
//     CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                                             // 0x13d840
//     BOOL bValue = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCMenuButton_OSMenu", bValue))       m_bOSMenu = bValue;        // 0x1409a0
//     bValue = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCMenuButton_RightArrow", bValue))   m_bRightArrow = bValue;
//     bValue = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCMenuButton_StayPressed", bValue))  m_bStayPressed = bValue;
//     bValue = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCMenuButton_DefaultClick", bValue)) m_bDefaultClick = bValue;
//     bValue = FALSE;
//     if (ReadBoolProp(tagManager, L"MFCMenuButton_Autosize", bValue) && bValue)
//         SizeToContent();                                                  // vslot 94 (+0x2f0), bCalcOnly = FALSE
//     return 0;                                                             // ~CTagManager 0x13d8f0, ~CString
// (The tag strings were read from the mfc140.dll twin's .rdata, where they
// are narrow; the mfc140u body passes the same names as wide strings.)
// Symbol: ?OnInitControl@CMFCMenuButton@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCMenuButton__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    S_MenuButton* s = M(pThis);

    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));
    alignas(16) unsigned char tagManagerStorage[64];
    std::memset(tagManagerStorage, 0, sizeof tagManagerStorage);
    impl___0CTagManager__QEAA_PEB_W_Z(tagManagerStorage, strDst.GetString());

    int bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(tagManagerStorage, L"MFCMenuButton_OSMenu", &bValue)) {
        s->m_bOSMenu = bValue;
    }
    bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(tagManagerStorage, L"MFCMenuButton_RightArrow", &bValue)) {
        s->m_bRightArrow = bValue;
    }
    bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(tagManagerStorage, L"MFCMenuButton_StayPressed", &bValue)) {
        s->m_bStayPressed = bValue;
    }
    bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(tagManagerStorage, L"MFCMenuButton_DefaultClick", &bValue)) {
        s->m_bDefaultClick = bValue;
    }
    bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(tagManagerStorage, L"MFCMenuButton_Autosize", &bValue) &&
        bValue != FALSE) {
        CSize sizeIgnored;
        DispatchSizeToContent(pThis, &sizeIgnored, FALSE);
    }

    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}

// Retail (entry RVA 0x8e6a0, mfc140u), fully transcribed:
//     if (((nChar - 0x20) & ~8) != 0)       // i.e. nChar is neither VK_SPACE (0x20) nor VK_DOWN (0x28)
//         { jmp CWnd::Default(); }          // 0x28ac80 (result discarded)
//     m_bClickOnMenu = TRUE;
//     OnShowMenu();                         // vslot 104 (+0x340), tail call
// nRepCnt and nFlags are not read.
// Symbol: ?OnKeyDown@CMFCMenuButton@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCMenuButton__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt;
    (void)nFlags;
    if (pThis == nullptr) return;
    if (((nChar - 0x20u) & ~8u) != 0) {
        impl__Default_CWnd__IEAA_JXZ(W(pThis));
        return;
    }
    M(pThis)->m_bClickOnMenu = TRUE;
    DispatchOnShowMenu(pThis);
}

// Retail (entry RVA 0x8e830, mfc140u), fully transcribed:
//     if (m_bStayPressed && m_bMenuIsActive && m_bPushed) {                 // +0xb34, +0xb40, +0x114
//         CWnd::Default();                                                  // 0x28ac80
//         if (m_bCaptured) { ::ReleaseCapture(); m_bCaptured = FALSE; }      // +0x120, import 0x2c7258
//         m_bClickiedInside = FALSE;                                        // +0x118
//         m_bHover = FALSE;                                                 // +0x124
//         return;
//     }
//     CMFCButton::OnKillFocus(pNewWnd);                                     // 0x1ec40, tail jump
// Symbol: ?OnKillFocus@CMFCMenuButton@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCMenuButton__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pNewWnd) {
    if (pThis == nullptr) return;
    S_MenuButton* s = M(pThis);
    if (s->m_bStayPressed && s->m_bMenuIsActive && s->m_bPushed) {
        impl__Default_CWnd__IEAA_JXZ(W(pThis));
        ReleaseCaptureIfCaptured(s);
        s->m_bClickiedInside = FALSE;
        s->m_bHover = FALSE;
        return;
    }
    impl__OnKillFocus_CMFCButton__IEAAXPEAVCWnd___Z(pThis, pNewWnd);
}

// Retail (entry RVA 0x8e8f0, mfc140u), fully transcribed:
//     if (!m_bMenuIsActive) {                                               // +0xb40
//         CMFCButton::OnLButtonDblClk(nFlags, point);                       // 0x1ecb0
//         m_bClickOnMenu = FALSE;                                           // +0xb44
//     }
//     if (m_bCaptured) { ::ReleaseCapture(); m_bCaptured = FALSE; }          // +0x120, import 0x2c7258
// Symbol: ?OnLButtonDblClk@CMFCMenuButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCMenuButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    S_MenuButton* s = M(pThis);
    if (!s->m_bMenuIsActive) {
        impl__OnLButtonDblClk_CMFCButton__IEAAXIVCPoint___Z(pThis, nFlags, PtFrom(point));
        s->m_bClickOnMenu = FALSE;
    }
    ReleaseCaptureIfCaptured(s);
}

// Retail (entry RVA 0x8e6d0, mfc140u), fully transcribed:
//     if (m_bMenuIsActive) { CWnd::Default(); return; }                     // +0xb40, 0x28ac80
//     m_bClickOnMenu = TRUE;                                                // +0xb44 (unconditional)
//     if (m_bDefaultClick) {                                                // +0xb3c
//         CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);          // import 0x2c7330
//         CRect rectArrow = rectClient;
//         rectArrow.left = rectArrow.right - CMenuImages::Size().cx - 10;   // 0x8f600
//         if (!::PtInRect(&rectArrow, point)) {                             // import 0x2c72f8
//             m_bClickOnMenu = FALSE;
//             m_nMenuResult = 0;                                            // +0xb30
//             CMFCButton::OnLButtonDown(nFlags, point);                     // 0x1e010
//             return;
//         }
//     }
//     SetFocus();                                                           // CWnd::SetFocus 0x2a9b60
//     OnShowMenu();                                                         // vslot 104 (+0x340)
// Symbol: ?OnLButtonDown@CMFCMenuButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCMenuButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    S_MenuButton* s = M(pThis);
    if (s->m_bMenuIsActive) {
        impl__Default_CWnd__IEAA_JXZ(W(pThis));
        return;
    }
    s->m_bClickOnMenu = TRUE;
    if (s->m_bDefaultClick) {
        CRect rectClient(0, 0, 0, 0);
        ::GetClientRect(s->m_hWnd, rectClient);
        CRect rectArrow = rectClient;
        CSize sizeArrow;
        impl__Size_CMenuImages__SA_AVCSize__XZ(&sizeArrow);
        rectArrow.left = rectArrow.right - sizeArrow.cx - 10;
        if (!::PtInRect(rectArrow, PtFrom(point))) {
            s->m_bClickOnMenu = FALSE;
            s->m_nMenuResult = 0;
            impl__OnLButtonDown_CMFCButton__IEAAXIVCPoint___Z(pThis, nFlags, PtFrom(point));
            return;
        }
    }
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(W(pThis));
    DispatchOnShowMenu(pThis);
}

// Retail (entry RVA 0x8e7c0, mfc140u), fully transcribed:
//     if (m_bStayPressed && m_bMenuIsActive && m_bPushed) {                 // +0xb34, +0xb40, +0x114
//         m_bClickiedInside = FALSE;                                        // +0x118
//         CWnd::Default();                                                  // 0x28ac80
//         if (m_bCaptured) { ::ReleaseCapture(); m_bCaptured = FALSE; }      // +0x120, import 0x2c7258
//         return;
//     }
//     if (!m_bClickOnMenu) CMFCButton::OnLButtonUp(nFlags, point);          // +0xb44, 0x1e100
// Symbol: ?OnLButtonUp@CMFCMenuButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCMenuButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    S_MenuButton* s = M(pThis);
    if (s->m_bStayPressed && s->m_bMenuIsActive && s->m_bPushed) {
        s->m_bClickiedInside = FALSE;
        impl__Default_CWnd__IEAA_JXZ(W(pThis));
        ReleaseCaptureIfCaptured(s);
        return;
    }
    if (!s->m_bClickOnMenu) {
        impl__OnLButtonUp_CMFCButton__IEAAXIVCPoint___Z(pThis, nFlags, PtFrom(point));
    }
}

// Retail (entry RVA 0x8e4e0, mfc140u), fully transcribed:
//     if (m_hMenu == NULL || m_bMenuIsActive) return;                       // +0xb28, +0xb40
//     CRect rectWindow;  ::GetWindowRect(m_hWnd, &rectWindow);              // import 0x2c6c08
//     int x, y;
//     if (m_bRightArrow) { x = rectWindow.right; y = rectWindow.top; }      // +0xb20
//     else               { x = rectWindow.left;  y = rectWindow.bottom; }
//     if (m_bStayPressed) { m_bPushed = TRUE; m_bHighlighted = TRUE; }      // +0xb34 -> +0x114, +0x11c
//     m_bMenuIsActive = TRUE;
//     ::InvalidateRect(m_hWnd, NULL, TRUE);                                 // import 0x2c7128
//     if (!m_bOSMenu && afxContextMenuManager != NULL) {                    // +0xb38, 0x3be1b0
//         m_nMenuResult = afxContextMenuManager->TrackPopupMenu(m_hMenu, x, y, this, FALSE);  // its vslot 7 (+0x38)
//         SetFocus();                                                       // 0x2a9b60
//     } else {
//         m_nMenuResult = ::TrackPopupMenu(m_hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,  // 0x180; import 0x2c71c0
//                                          x, y, 0, m_hWnd, NULL);
//     }
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));                // import 0x2c72d8, 0x28ad70
//     if (m_nMenuResult != 0 && pParent != NULL)
//         ::SendMessage(pParent->m_hWnd, WM_COMMAND,                        // import 0x2c7120 (SendMessageW)
//                       (WORD)GetDlgCtrlID(), (LPARAM)m_hWnd);              // 0x2a99a0
//     m_bPushed = FALSE;  m_bHighlighted = FALSE;  m_bMenuIsActive = FALSE;
//     ::InvalidateRect(m_hWnd, NULL, TRUE);
//     ::UpdateWindow(m_hWnd);                                               // import 0x2c7300
//     if (m_bCaptured) { ::ReleaseCapture(); m_bCaptured = FALSE; }          // import 0x2c7258
// DEVIATIONS, each deliberate:
//  * afxContextMenuManager is replaced by CurrentContextMenuManager() (see its
//    comment above): a manager constructed outside the CWinAppEx is not seen.
//  * The manager's TrackPopupMenu goes to the exported DLL thunk, not through
//    retail's MSVC vtable slot 7 (+0x38).  That thunk
//    (featurepack/menu/CContextMenuManager.cpp) re-dispatches with a C++
//    virtual call on OpenMFC's own (mingw) CContextMenuManager vtable: for the
//    manager OpenMFC's InitContextMenuManager creates that reaches the same
//    function, but a manager whose vptr is a client MSVC vftable is not
//    dispatched through retail's slot 7, so its override is not reliably reached.
// Symbol: ?OnShowMenu@CMFCMenuButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowMenu_CMFCMenuButton__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MenuButton* s = M(pThis);
    if (s->m_hMenu == nullptr || s->m_bMenuIsActive) return;

    CRect rectWindow(0, 0, 0, 0);
    ::GetWindowRect(s->m_hWnd, rectWindow);
    int x = 0;
    int y = 0;
    if (s->m_bRightArrow) {
        x = rectWindow.right;
        y = rectWindow.top;
    } else {
        x = rectWindow.left;
        y = rectWindow.bottom;
    }
    if (s->m_bStayPressed) {
        s->m_bPushed = TRUE;
        s->m_bHighlighted = TRUE;
    }
    s->m_bMenuIsActive = TRUE;
    ::InvalidateRect(s->m_hWnd, nullptr, TRUE);

    CContextMenuManager* pManager = CurrentContextMenuManager();
    if (!s->m_bOSMenu && pManager != nullptr) {
        s->m_nMenuResult = static_cast<int>(
            impl__TrackPopupMenu_CContextMenuManager__UEAAIPEAUHMENU____HHPEAVCWnd__H_Z(
                pManager, s->m_hMenu, x, y, W(pThis), FALSE));
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(W(pThis));
    } else {
        s->m_nMenuResult = ::TrackPopupMenu(s->m_hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD,
                                            x, y, 0, s->m_hWnd, nullptr);
    }

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(s->m_hWnd));
    if (s->m_nMenuResult != 0 && pParent != nullptr) {
        HWND hWndSelf = s->m_hWnd;
        const int nID = impl__GetDlgCtrlID_CWnd__QEBAHXZ(W(pThis));
        ::SendMessage(pParent->m_hWnd, WM_COMMAND,
                      static_cast<WPARAM>(static_cast<WORD>(nID)),
                      reinterpret_cast<LPARAM>(hWndSelf));
    }

    s->m_bPushed = FALSE;
    s->m_bHighlighted = FALSE;
    s->m_bMenuIsActive = FALSE;
    ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    ::UpdateWindow(s->m_hWnd);
    ReleaseCaptureIfCaptured(s);
}

// Retail (entry RVA 0x8e8a0, mfc140u), fully transcribed:
//     if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&
//         CMFCPopupMenu::m_pActivePopupMenu == NULL) {                      // 0x3be288
//         m_bClickOnMenu = TRUE;                                            // +0xb44
//         OnShowMenu();                                                     // vslot 104 (+0x340)
//         return TRUE;
//     }
//     return CMFCButton::PreTranslateMessage(pMsg);                         // 0x1e560, tail jump
// Symbol: ?PreTranslateMessage@CMFCMenuButton@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCMenuButton__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN &&
        impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA == nullptr) {
        M(pThis)->m_bClickOnMenu = TRUE;
        DispatchOnShowMenu(pThis);
        return TRUE;
    }
    return impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}

// Retail (entry RVA 0x28270, mfc140u), fully transcribed.  The CSize return
// travels through the hidden pointer in RDX (RCX = this, R8D = bCalcOnly):
//     CSize size = CMFCButton::SizeToContent(FALSE);     // 0x1e280 -- FALSE, not bCalcOnly
//     size.cx += CMenuImages::Size().cx;                 // 0x8f600
//     if (!bCalcOnly)
//         SetWindowPos(NULL, -1, -1, size.cx, size.cy,
//                      SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);  // 0x16; CWnd::SetWindowPos 0x2a9a60
//     return size;
// Retail always calls the base with bCalcOnly == FALSE (R8D is zeroed before
// the call), so the base may resize the window itself before the widened
// size is applied here; that is reproduced as is.
// Symbol: ?SizeToContent@CMFCMenuButton@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCMenuButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly) {
    if (pRet == nullptr) return pRet;
    if (pThis == nullptr) { *pRet = CSize(0, 0); return pRet; }
    impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(pThis, pRet, FALSE);
    CSize sizeArrow;
    impl__Size_CMenuImages__SA_AVCSize__XZ(&sizeArrow);
    pRet->cx += sizeArrow.cx;
    if (!bCalcOnly) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(W(pThis), nullptr, -1, -1, pRet->cx, pRet->cy,
                                                   SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return pRet;
}
