// CMFCDesktopAlertWnd — OpenMFC implementation.
//
// The desktop-alert ("toast") popup: a WS_POPUP CWnd that hosts a
// CMFCDesktopAlertDialog child, paints its own caption through the visual
// manager, slides/fades itself in and out on a timer, and can be dragged by
// that caption.  afxdesktopalertwnd.h:62 declares the class; this file only
// has to reproduce the exported entry points.
//
// Every IMPLEMENTED body below was transcribed from the retail disassembly
// (disas.py); the five symbols left stubbed say so at their definitions.
// RVAs quoted as "(mfc140)" are FUNCTION ENTRIES in mfc140.dll, the ANSI twin
// of mfc140u.dll that disas.py reads by default; function bodies are
// byte-identical between the two images, so control flow, member offsets and
// constants are correct for mfc140u as well, but the ADDRESSES are not
// mfc140u addresses and must never be quoted as such.  Where a specific
// instruction is meant, the comment says so.  Every USER32 import slot named
// below was resolved with iat.py; `call *0x1802c5bd0` in the listings is the
// Control Flow Guard dispatcher (__guard_dispatch_icall_fptr), i.e. an
// indirect call through RAX, so every "vftable slot N" cited here is
// (vtable byte offset / 8) read off such a call site.
//
// NOTE on ANSI/Unicode imports: the listings show SendMessageA / PostMessageA
// / GetMonitorInfoA / SystemParametersInfoA / LoadCursorA because the default
// image is the ANSI twin; mfc140u imports the W entry points at the same
// slots.  The code below writes the TCHAR-generic ::SendMessage etc. and lets
// UNICODE (always defined for this build) pick the W flavour.

// This TU deliberately does NOT include openmfc/afxwin.h.  Pulling CWnd's
// definition in emits CWnd/CCmdTarget/CWinThread inline vtables (and their
// typeinfo) into every object file that sees it, which shows up as a pile of
// newly undefined C++ symbols in this file's link audit.  Everything here is
// addressed by raw byte offset off the object base anyway, so the thunk
// declarations below spell class pointers `void*`: extern "C" linkage means
// the symbol name is unaffected and a pointer parameter is ABI-identical.
#include <windows.h>
#include <cstddef>
#include <cstring>
#include <ctime>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// ---------------------------------------------------------------------------
// Retail layout
// ---------------------------------------------------------------------------
// OpenMFC's public declaration (include/openmfc/afxmfc.h:1741) is
// `class CMFCDesktopAlertWnd : public CWnd { char _pad[16]; }` -- 0xf8 bytes,
// far too small.  The real size is 0x1810 (6160), which is exactly what this
// tree's own CRuntimeClass descriptor already records
// (phase4/src/featurepack/controls/RuntimeClasses.cpp:33,
//  MFC_DIALOGS_CTRLS_DESC(CMFCDesktopAlertWnd, 6160, ...)) and what the retail
// descriptor carries: GetThisClass (RVA 0x3c630, mfc140) returns
// &0x1802e2078, whose m_nObjectSize field reads 0x1810.
//
// Every offset below was read off the retail constructor
// ??0CMFCDesktopAlertWnd@@QEAA@XZ (RVA 0x3c750, mfc140) and cross-checked
// against the destructor (RVA 0x3c8f0) and against the member ORDER declared
// in afxdesktopalertwnd.h:62.  The constructor, fully transcribed:
//
//     CWnd::CWnd();                                   // 0x288b10
//     vfptr = &CMFCDesktopAlertWnd::`vftable';        // 0x1802e2268
//     m_ptLastPos = m_ptStartMove = (0,0);            // +0xf0 / +0xf8
//     CMFCButton::CMFCButton(&m_btnClose);            // 0x1c9e0, this+0x138
//       m_btnClose.vfptr = CMFCDesktopAlertWndButton vftable (0x1802e2580)
//       m_btnClose.m_bIsCaptionButton = m_bIsCloseButton = 0;   // +0xb20
//     CMFCButton::CMFCButton(&m_btnMenu);             // this+0xc60, same two
//     CBitmap::CBitmap(&m_bmpScreenSrc/Dst/Tmp);      // +0x17c8/+0x17d8/+0x17e8
//     m_pWndOwner       = NULL;                       // +0xe8
//     m_bIsActive       = 0;                          // +0x104
//     m_nAutoCloseTime  = 3000;   // 0xbb8            // +0x114
//     m_bSmallCaption   = 1;                          // +0x10c
//     m_bHasCloseButton = 1;                          // +0x108
//     m_hMenu           = NULL;                       // +0x128
//     m_pWndDlg         = NULL;                       // +0x130
//     m_uiDlgResID      = 0;                          // +0x110
//     m_bMoving         = 0;                          // +0x100
//     m_nBtnMarginVert  = m_nBtnMarginHorz = 2;       // +0x118 / +0x11c
//     m_nTransparency   = 0xff;                       // +0x120
//     m_AnimationType   = 0;                          // +0x1788
//     m_AnimationSpeed  = 30; m_nAnimationAlpha = 0;  // +0x178c / +0x1790
//     m_bAnimationIsDone = m_bIsAnimRight = 0;        // +0x1794 / +0x1798
//     m_bIsAnimDown = m_bFadeOutAnimation = 0;        // +0x179c / +0x17a0
//     m_AnimSize = m_FinalSize = (0,0);               // +0x17b4 / +0x17bc
//     m_ptLastPos = m_ptStartMove = (-1,-1); m_bMoving = 0;   // re-stored last
//
// (m_iFadePercent/m_iFadeStep/m_nAnimStepX/m_nAnimStepY at +0x17a4..+0x17b3
//  and m_cFade*Bits at +0x17f8..+0x180f are NOT initialised by the retail
//  constructor; StartAnimation sets the first four before anything reads
//  them.  The destructor does not free m_cFade*Bits either.)
//
// This is a FILE-LOCAL view keyed off the CWnd base, exactly as
// featurepack/controls/CMFCListCtrl.cpp and featurepack/menu/CMFCPopupMenu.cpp
// do for their classes; no public header is touched.  See `headerRequests`.
struct AlertWndLayout {
    unsigned char _cwnd[0xe8];          // 0x0000  CWnd base
    void*         m_pWndOwner;          // 0x00e8  CWnd*
    POINT         m_ptLastPos;          // 0x00f0
    POINT         m_ptStartMove;        // 0x00f8
    int           m_bMoving;            // 0x0100
    int           m_bIsActive;          // 0x0104
    int           m_bHasCloseButton;    // 0x0108
    int           m_bSmallCaption;      // 0x010c
    unsigned int  m_uiDlgResID;         // 0x0110
    int           m_nAutoCloseTime;     // 0x0114  milliseconds
    int           m_nBtnMarginVert;     // 0x0118
    int           m_nBtnMarginHorz;     // 0x011c
    unsigned char m_nTransparency;      // 0x0120  BYTE (0..255)
    unsigned char _pad0121[7];          // 0x0121
    HMENU         m_hMenu;              // 0x0128
    void*         m_pWndDlg;            // 0x0130  CMFCDesktopAlertDialog*
    unsigned char m_btnClose[0xb28];    // 0x0138  CMFCDesktopAlertWndButton
    unsigned char m_btnMenu[0xb28];     // 0x0c60  CMFCDesktopAlertWndButton
    int           m_AnimationType;      // 0x1788  CMFCPopupMenu::ANIMATION_TYPE
    unsigned int  m_AnimationSpeed;     // 0x178c
    unsigned int  m_nAnimationAlpha;    // 0x1790
    int           m_bAnimationIsDone;   // 0x1794
    int           m_bIsAnimRight;       // 0x1798
    int           m_bIsAnimDown;        // 0x179c
    int           m_bFadeOutAnimation;  // 0x17a0
    int           m_iFadePercent;       // 0x17a4
    int           m_iFadeStep;          // 0x17a8
    int           m_nAnimStepX;         // 0x17ac
    int           m_nAnimStepY;         // 0x17b0
    SIZE          m_AnimSize;           // 0x17b4
    SIZE          m_FinalSize;          // 0x17bc
    unsigned char _pad17c4[4];          // 0x17c4  (alignment before the CBitmaps)
    void*         m_bmpScreenSrc[2];    // 0x17c8  CBitmap { vfptr, m_hObject }
    void*         m_bmpScreenDst[2];    // 0x17d8
    void*         m_bmpScreenTmp[2];    // 0x17e8
    void*         m_cFadeSrcBits;       // 0x17f8  COLORREF*
    void*         m_cFadeDstBits;       // 0x1800
    void*         m_cFadeTmpBits;       // 0x1808
};                                      // 0x1810

// (sizeof(CWnd) == 0xe8 and offsetof(CWnd, m_hWnd) == 0x40 are pinned by
//  static_asserts in phase4/src/featurepack/controls/CMFCListCtrl.cpp:129-130
//  and in half a dozen sibling files; they cannot be re-asserted here without
//  including afxwin.h, which this TU deliberately does not do -- see the top.)
static_assert(offsetof(AlertWndLayout, m_pWndOwner)       == 0x00e8, "ctor 0x3c7ee");
static_assert(offsetof(AlertWndLayout, m_ptLastPos)       == 0x00f0, "ctor 0x3c86e/0x3c874");
static_assert(offsetof(AlertWndLayout, m_ptStartMove)     == 0x00f8, "ctor 0x3c880/0x3c886");
static_assert(offsetof(AlertWndLayout, m_bMoving)         == 0x0100, "ctor 0x3c87a");
static_assert(offsetof(AlertWndLayout, m_bIsActive)       == 0x0104, "ctor 0x3c7f5");
static_assert(offsetof(AlertWndLayout, m_bHasCloseButton) == 0x0108, "ctor 0x3c80e");
static_assert(offsetof(AlertWndLayout, m_bSmallCaption)   == 0x010c, "ctor 0x3c808");
static_assert(offsetof(AlertWndLayout, m_uiDlgResID)      == 0x0110, "ctor 0x3c822");
static_assert(offsetof(AlertWndLayout, m_nAutoCloseTime)  == 0x0114, "ctor 0x3c7fb (=3000)");
static_assert(offsetof(AlertWndLayout, m_nBtnMarginVert)  == 0x0118, "ctor 0x3c82b (=2)");
static_assert(offsetof(AlertWndLayout, m_nBtnMarginHorz)  == 0x011c, "ctor 0x3c831 (=2)");
static_assert(offsetof(AlertWndLayout, m_nTransparency)   == 0x0120, "ctor 0x3c837 (=0xff)");
static_assert(offsetof(AlertWndLayout, m_hMenu)           == 0x0128, "ctor 0x3c814");
static_assert(offsetof(AlertWndLayout, m_pWndDlg)         == 0x0130, "ctor 0x3c81b");
static_assert(offsetof(AlertWndLayout, m_btnClose)        == 0x0138, "ctor 0x3c784");
static_assert(offsetof(AlertWndLayout, m_btnMenu)         == 0x0c60, "ctor 0x3c7a4");
static_assert(offsetof(AlertWndLayout, m_AnimationType)   == 0x1788, "ctor 0x3c83e");
static_assert(offsetof(AlertWndLayout, m_AnimationSpeed)  == 0x178c, "ctor 0x3c844 (=30)");
static_assert(offsetof(AlertWndLayout, m_nAnimationAlpha) == 0x1790, "ctor 0x3c844 (8-byte store)");
static_assert(offsetof(AlertWndLayout, m_bAnimationIsDone)== 0x1794, "ctor 0x3c84f");
static_assert(offsetof(AlertWndLayout, m_bIsAnimRight)    == 0x1798, "StartAnimation 0x3ddb4");
static_assert(offsetof(AlertWndLayout, m_bIsAnimDown)     == 0x179c, "StartAnimation 0x3ddd1");
static_assert(offsetof(AlertWndLayout, m_bFadeOutAnimation)==0x17a0, "StartAnimation 0x3dd61");
static_assert(offsetof(AlertWndLayout, m_iFadePercent)    == 0x17a4, "StartAnimation 0x3ddde");
static_assert(offsetof(AlertWndLayout, m_iFadeStep)       == 0x17a8, "StartAnimation 0x3ddf2");
static_assert(offsetof(AlertWndLayout, m_nAnimStepX)      == 0x17ac, "StartAnimation 0x3de16");
static_assert(offsetof(AlertWndLayout, m_nAnimStepY)      == 0x17b0, "StartAnimation 0x3de38");
static_assert(offsetof(AlertWndLayout, m_AnimSize)        == 0x17b4, "ctor 0x3c856");
static_assert(offsetof(AlertWndLayout, m_FinalSize)       == 0x17bc, "ctor 0x3c85d");
static_assert(offsetof(AlertWndLayout, m_bmpScreenSrc)    == 0x17c8, "ctor 0x3c7cb");
static_assert(offsetof(AlertWndLayout, m_bmpScreenDst)    == 0x17d8, "ctor 0x3c7d9");
static_assert(offsetof(AlertWndLayout, m_bmpScreenTmp)    == 0x17e8, "ctor 0x3c7e7");
static_assert(sizeof(AlertWndLayout) == 0x1810, "CRuntimeClass m_nObjectSize == 0x1810");

inline AlertWndLayout*       L(void* p)       { return static_cast<AlertWndLayout*>(p); }
inline const AlertWndLayout* L(const void* p) { return static_cast<const AlertWndLayout*>(p); }

// Every CWnd in play -- this window, the two embedded caption buttons, the
// child dialog -- keeps its HWND at +0x40 of its own object, which is what the
// CWnd::GetSafeHwnd that retail inlines everywhere reads.
inline HWND HWndOf(const void* p) {
    if (p == nullptr) return nullptr;
    return *reinterpret_cast<HWND const*>(static_cast<unsigned char const*>(p) + 0x40);
}

// ---------------------------------------------------------------------------
// Timer ids the retail bodies use (literals in the disassembly, no symbol)
// ---------------------------------------------------------------------------
constexpr UINT_PTR kTimerAutoClose = 0xec0e;   // OnCancelMode 0x3e98d, OnTimer 0x3d185
constexpr UINT_PTR kTimerAnimate   = 0xec0f;   // StartAnimation 0x3dea2, OnTimer 0x3d3e9
constexpr UINT_PTR kTimerActive    = 0xec10;   // OnTimer: kTimerAnimate + 1 (0x3d19d)

// CMFCPopupMenu::ANIMATION_TYPE, as the retail comparisons spell it:
//   0 NO_ANIMATION, 1 UNFOLD, 2 SLIDE, 3 FADE, 0x3e7 SYSTEM_DEFAULT_ANIMATION.
// (0x3e7 == 999 is the value StartAnimation/OnTimer compare against before
//  substituting the global menu-animation settings, exactly as the inline
//  GetActualAnimationType in afxdesktopalertwnd.h:162 does.)
constexpr int kAnimNone   = 0;
constexpr int kAnimUnfold = 1;
constexpr int kAnimSlide  = 2;
constexpr int kAnimFade   = 3;
constexpr int kAnimSystem = 0x3e7;

// ---------------------------------------------------------------------------
// afxGlobalData, read the way every other retail body inlines it:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }
// The offsets are (retail absolute VA - the gate's VA 0x1803ba380 in mfc140,
// which is ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A in the mfc140 symbol map):
//   +0x25c  bIsRemoteSession   -- StartAnimation refuses to animate when set
//   +0x26c  m_bMenuAnimation   -- StartAnimation reads it at 0x3dcbd and
//   +0x270  m_bMenuFadeEffect  -- this one at 0x3dcdf, in exactly the order
//           the public inline GetActualAnimationType uses them.
//   +0x288  m_nBitsPerPixel    -- compared with 8 before allowing a fade / a
//           layered window.
// The names follow from the member order of AFX_GLOBAL_DATA in
// atlmfc/include/afxglobals.h:160-176 (bIsWindows7, bIsRemoteSession,
// m_bIsBlackHighContrast, m_bIsWhiteHighContrast, m_bUseBuiltIn32BitIcons,
// m_bMenuAnimation, m_bMenuFadeEffect, m_bIsRTL, m_bEnableAccessibility,
// m_bUnderlineKeyboardShortcuts, m_bSysUnderlineKeyboardShortcuts,
// m_bRefreshAutohideBars, m_nBitsPerPixel -- consecutive 4-byte fields)
// anchored at +0x258 == bIsWindows7 / +0x260 == m_bIsBlackHighContrast,
// which CMFCToolBarComboBoxButton.cpp:302-303 already uses.
constexpr int kGlobalDataInitGate       = 0x000;
constexpr int kGlobalDataIsRemoteSess   = 0x25c;
constexpr int kGlobalDataMenuAnimation  = 0x26c;
constexpr int kGlobalDataMenuFadeEffect = 0x270;
constexpr int kGlobalDataBitsPerPixel   = 0x288;

} // namespace

// ---------------------------------------------------------------------------
// Thunks this file calls.  C++ methods of this DLL exist only as impl__
// thunks, so every cross-class call below goes through one.
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(void* pThis);                      // core/window/Thunks.cpp:1183
extern "C" void    MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(void* pThis);                     // core/window/Thunks.cpp:1503
extern "C" void    MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(void* pThis);                   // core/window/Thunks.cpp:1568
extern "C" void    MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const void* pThis, RECT* pRect); // core/window/Thunks.cpp:1658
extern "C" int     MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(void* pThis, WPARAM wParam, LPARAM lParam);    // core/window/CWnd.cpp:1374
extern "C" int     MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(void* pThis, int nCmdShow);     // core/window/CWnd.cpp:770
extern "C" int     MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(void* pThis);                 // core/window/CWnd.cpp:793
extern "C" int     MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    void* pThis, const void* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);          // core/window/CWnd.cpp
extern "C" SIZE*   MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(SIZE* pRet);   // CSize is tagSIZE + inline members            // featurepack/controls/CMFCReBar.cpp:1244
extern "C" void*   MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(void* pThis, void* pWnd);   // core/gdi/CPaintDC.cpp:12
extern "C" void    MS_ABI impl___1CPaintDC__UEAA_XZ(void* pThis);                         // core/gdi/CPaintDC.cpp:30
extern "C" void    MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);         // featurepack/CMFC_misc_stubs.cpp
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                  // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" unsigned int  impl__AFX_WM_ON_CLOSEPOPUPWINDOW__3IA;                           // core/runtime/Globals.cpp:125

// CMFCDesktopAlertDialog::HasFocus -- featurepack/controls/CMFCDesktopAlertDialog.cpp:36
// is still a generated stub whose parameter list LOST the `this` pointer
// (`int impl__HasFocus_...()`).  The declaration here is the one the mangled
// name describes; the stub ignores RCX and returns 0, so the call is harmless
// today but the definition needs repairing (see `headerRequests`).
extern "C" int MS_ABI impl__HasFocus_CMFCDesktopAlertDialog__QEBAHXZ(const void* pThis);

// CDialogTemplate -- both of these are generated stubs in
// phase4/src/core/dialog/CDialogTemplate.cpp (:66 lost the `this` pointer,
// :53 is an empty body).  Declared here with the signature the mangled names
// describe; see `headerRequests`.
extern "C" int  MS_ABI impl__Load_CDialogTemplate__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszResource);
extern "C" void MS_ABI impl__GetSizeInPixels_CDialogTemplate__QEBAXPEAUtagSIZE___Z(const void* pThis, SIZE* pSize);

// This file's own thunks, called by its siblings below.
extern "C" void MS_ABI impl__OnCancelMode_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnDraw_CMFCDesktopAlertWnd__UEAAXPEAVCDC___Z(void* pThis, void* pDC);
extern "C" void MS_ABI impl__DrawAnimation_CMFCDesktopAlertWnd__IEAAXPEAVCDC___Z(void* pThis, void* pDC);
extern "C" int  MS_ABI impl__GetCaptionHeight_CMFCDesktopAlertWnd__UEAAHXZ(void* pThis);
extern "C" void* MS_ABI impl__GetCaptionRect_CMFCDesktopAlertWnd__IEAA_AVCRect__XZ(void* pThis, RECT* pRet);
extern "C" int  MS_ABI impl__ProcessCommand_CMFCDesktopAlertWnd__QEAAHPEAUHWND_____Z(void* pThis, HWND hwnd);
extern "C" void MS_ABI impl__StartAnimation_CMFCDesktopAlertWnd__IEAAXH_Z(void* pThis, int bShow);
extern "C" void MS_ABI impl__StartWindowMove_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis);

namespace {

// The retail one-time gate, as every reader inlines it.
inline void EnsureGlobalData() {
    int gate;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        gate = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &gate, sizeof gate);
    }
}
inline int GlobalDataInt(int off) {
    int v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}

// afxdesktopalertwnd.h:162 GetActualAnimationType, inlined into StartAnimation
// (0x3dca0..0x3dcee) and into OnTimer (0x3d12a..0x3d182) identically.
inline int ActualAnimationType(const AlertWndLayout* p) {
    int type = p->m_AnimationType;
    if (type != kAnimSystem) return type;
    EnsureGlobalData();
    if (GlobalDataInt(kGlobalDataMenuAnimation) == 0) return kAnimNone;
    EnsureGlobalData();
    return GlobalDataInt(kGlobalDataMenuFadeEffect) ? kAnimFade : kAnimSlide;
}

// Retail keeps the tick of the previous animation step in a DLL-global int
// (the `mov %eax,0x1803bb968` at the end of StartAnimation 0x3deb3 and the
// `mov %esi,0x1803bb968` at OnTimer 0x3d52f, both read back at OnTimer
// 0x3d293).  It is not an exported symbol, so OpenMFC keeps it here.
// DEVIATION: file-local rather than DLL-global -- identical behaviour because
// the only two readers/writers in the retail image are those two functions,
// both of which live in this file.
int g_nLastAnimationTick = 0;

// `CPoint` / `CSize` passed BY VALUE are 8-byte aggregates in one register
// under the MS ABI; the thunk signatures below model them as `long long`.
inline POINT PointFromArg(long long v) {
    POINT pt;
    pt.x = static_cast<int>(static_cast<unsigned int>(static_cast<unsigned long long>(v) & 0xffffffffu));
    pt.y = static_cast<int>(static_cast<unsigned int>(static_cast<unsigned long long>(v) >> 32));
    return pt;
}

// The "stop dragging" tail shared verbatim by OnLButtonUp (0x3e8b9..0x3e917)
// and OnCancelMode (0x3e961..0x3e9bc) -- same instructions, same order.
void StopWindowMove(AlertWndLayout* p) {
    if (p->m_bMoving == 0) return;
    ::ReleaseCapture();
    const int nAutoClose = p->m_nAutoCloseTime;
    p->m_bMoving = FALSE;
    if (nAutoClose > 0) {
        ::SetTimer(HWndOf(p), kTimerAutoClose, static_cast<UINT>(nAutoClose), nullptr);
    }
    RECT rect = { 0, 0, 0, 0 };
    ::GetWindowRect(HWndOf(p), &rect);
    p->m_ptLastPos.x = rect.left;      // the 8-byte store at 0x3e9b5 / 0x3e910
    p->m_ptLastPos.y = rect.top;       // copies rect.TopLeft() into m_ptLastPos
}

} // namespace

// ===========================================================================
// Construction
// ===========================================================================

// Retail RVA 0x3ca50 (mfc140), 1568 bytes.  NOT transcribed -- see the
// `stubbed` report.  What it does, in order, for whoever picks this up:
//   * m_ptLastPos = ptPos; nCaption = GetCaptionHeight() (vftable slot 96,
//     +0x300); remembers ::GetFocus() / ::GetForegroundWindow() as CWnd*s;
//   * AfxRegisterWndClass(CS_SAVEBITS 0x800, ::LoadCursor(NULL, IDC_ARROW),
//     (HBRUSH)(COLOR_BTNFACE + 1), NULL)  (0x28a920);
//   * CWnd::CreateEx (0x2898c0) with dwExStyle 0x88
//     (WS_EX_TOPMOST|WS_EX_TOOLWINDOW, plus WS_EX_LAYERED 0x80000 when
//     afxGlobalData.m_nBitsPerPixel > 8 and m_nTransparency < 255),
//     dwStyle WS_POPUP (0x80000000), an empty window name and an empty rect;
//   * resource-id form (m_uiDlgResID != 0): the dialog's own Create (dialog
//     vftable slot 91, +0x2d8), then this->GetDialogSize() (slot 93, +0x2e8);
//     params form: CMFCDesktopAlertDialog::CreateFromParams (0x3bfb0), size
//     read back from dialog +0xea8;
//   * sizes m_FinalSize from that plus the caption and button margins,
//     clamped to the monitor work area (MonitorFromPoint/GetMonitorInfo,
//     SPI_GETWORKAREA fallback);
//   * creates m_btnClose / m_btnMenu through the button's vftable slot 91
//     with WS_CHILD|WS_VISIBLE and CMFCButton::SetStdImage (0x1de30);
//   * OnBeforeShow (slot 94), SetWindowPos(&wndTopMost, ..., 0x50),
//     StartAnimation(TRUE), positions the dialog, SetTimer(0xec10, 100 ms),
//     and restores the remembered focus.
// It cannot be written honestly here while CMFCDesktopAlertDialog is a set of
// null stubs (featurepack/controls/CMFCDesktopAlertDialog.cpp).
// Symbol: ?CommonCreate@CMFCDesktopAlertWnd@@IEAAHVCPoint@@PEAVCMFCDesktopAlertWndInfo@@@Z
extern "C" int MS_ABI impl__CommonCreate_CMFCDesktopAlertWnd__IEAAHVCPoint__PEAVCMFCDesktopAlertWndInfo___Z(
    void* pThis, long long ptPos, void* pParams) {
    (void)pThis; (void)ptPos; (void)pParams;
    return 0;
}

// Retail RVA 0x3c9e0 (mfc140):
//     m_hMenu = hMenu; m_pWndOwner = pWndOwner;
//     m_pWndDlg = new CMFCDesktopAlertDialog;          // operator new 0x2840
//     m_pWndDlg-><+0xeb0> = 1;                          // "built from params"
//     return CommonCreate(ptPos, &params);              // tail call to 0x3ca50
// Left a stub because CommonCreate is (see above), and because allocating a
// 0xec0-byte CMFCDesktopAlertDialog whose constructor thunk
// (featurepack/controls/CMFCDesktopAlertDialog.cpp:7 -- `return pThis;`,
// initialising nothing) leaves a half-built object reachable from m_pWndDlg.
// Symbol: ?Create@CMFCDesktopAlertWnd@@UEAAHPEAVCWnd@@AEAVCMFCDesktopAlertWndInfo@@PEAUHMENU__@@VCPoint@@@Z
extern "C" int MS_ABI impl__Create_CMFCDesktopAlertWnd__UEAAHPEAVCWnd__AEAVCMFCDesktopAlertWndInfo__PEAUHMENU____VCPoint___Z(
    void* pThis, void* pWndOwner, void* params, HMENU hMenu, long long ptPos) {
    (void)pThis; (void)pWndOwner; (void)params; (void)hMenu; (void)ptPos;
    return 0;
}

// Retail RVA 0x3c980 (mfc140):
//     if (pRTIDlgBar == NULL) AfxThrowInvalidArgException();   // 0x225b80
//     m_hMenu = hMenu; m_pWndOwner = pWndOwner; m_uiDlgResID = uiDlgResID;
//     m_pWndDlg = (CMFCDesktopAlertDialog*)pRTIDlgBar->CreateObject();  // 0x233380
//     if (m_pWndDlg == NULL) AfxThrowInvalidArgException();
//     return CommonCreate(ptPos, NULL);                        // tail call
// Same reason as above.
// Symbol: ?Create@CMFCDesktopAlertWnd@@UEAAHPEAVCWnd@@IPEAUHMENU__@@VCPoint@@PEAUCRuntimeClass@@@Z
extern "C" int MS_ABI impl__Create_CMFCDesktopAlertWnd__UEAAHPEAVCWnd__IPEAUHMENU____VCPoint__PEAUCRuntimeClass___Z(
    void* pThis, void* pWndOwner, unsigned int uiDlgResID, HMENU hMenu, long long ptPos, void* pRTIDlgBar) {
    (void)pThis; (void)pWndOwner; (void)uiDlgResID; (void)hMenu; (void)ptPos; (void)pRTIDlgBar;
    return 0;
}

// ===========================================================================
// Animation
// ===========================================================================

// Retail RVA 0x3dfe0 (mfc140), ~1.6 KB.  NOT transcribed -- see the `stubbed`
// report.  It works with the three CBitmap members (m_bmpScreenSrc/Dst/Tmp at
// +0x17c8..) and the m_cFade*Bits DIB buffers (+0x17f8..) through a series of
// CDC / CGdiObject thunks and GDI imports to render the frame at the current
// m_AnimSize / m_iFadePercent.  Not reproduced: the body is long, drives
// several CDC members through this tree's non-MSVC CDC layout, and a
// partially-correct blit path would corrupt the paint rather than no-op.
// Symbol: ?DrawAnimation@CMFCDesktopAlertWnd@@IEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DrawAnimation_CMFCDesktopAlertWnd__IEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
}

// Retail RVA 0x3dc60 (mfc140), fully transcribed.
//
//     int type = GetActualAnimationType();                       // 0x3dca0
//     BOOL bAnimate = (type != NO_ANIMATION)                     // 0x3dcee
//                  && !afxGlobalData.<+0x25c>                    // 0x3dd0b
//                  && !(type == FADE && afxGlobalData.<+0x288> <= 8);  // 0x3dd30
//     if (bAnimate) {
//         m_bAnimationIsDone  = FALSE;                           // 0x3dd3f
//         m_bFadeOutAnimation = (bShow == 0);                    // 0x3dd51 sete
//         ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);  // 0x3dd6a
//         ::GetWindowRect(m_hWnd, &rectWindow);                  // 0x3dd82
//         m_bIsAnimRight = rectWindow.left < (rectScreen.left + rectScreen.right) / 2;
//         m_bIsAnimDown  = rectWindow.top  < (rectScreen.top + rectScreen.bottom) / 2;
//         m_iFadePercent = m_bFadeOutAnimation ? 100 : 0;        // 0x3ddde
//         m_iFadeStep    = m_bFadeOutAnimation ? -10 : 10;       // 0x3ddf2
//         if (m_FinalSize.cx > m_FinalSize.cy) {                 // 0x3ddf8
//             m_nAnimStepY = 10;
//             m_nAnimStepX = max(1, m_FinalSize.cx * 10 / m_FinalSize.cy);
//         } else {
//             m_nAnimStepX = 10;
//             m_nAnimStepY = max(1, m_FinalSize.cy * 10 / m_FinalSize.cx);
//         }
//         m_AnimSize = m_FinalSize;                              // 0x3de41
//         if (bShow) {                                           // 0x3de4f
//             if (type == UNFOLD) m_AnimSize.cx = m_nAnimStepX;  // falls through
//             if (type == UNFOLD || type == SLIDE) m_AnimSize.cy = m_nAnimStepY;
//         }
//         if (m_pWndDlg != NULL && ::IsWindowVisible(m_pWndDlg->m_hWnd))
//             m_pWndDlg->ShowWindow(SW_HIDE);                    // 0x3de8f
//         ::SetTimer(m_hWnd, 0xec0f, m_AnimationSpeed, NULL);    // 0x3dea7
//         g_nLastAnimationTick = clock();                        // 0x3dead
//     } else if (!bShow) {
//         ::SendMessage(m_hWnd, WM_CLOSE, 0, 0);                 // 0x3decf
//     } else {
//         if (m_btnClose.m_hWnd) m_btnClose.ShowWindow(SW_SHOWNOACTIVATE); // 0x3def4, edx=4
//         if (m_btnMenu.m_hWnd)  m_btnMenu.ShowWindow(SW_SHOWNOACTIVATE);  // 0x3df0e
//         m_bAnimationIsDone = TRUE;                             // 0x3df1a
//         if (m_nAutoCloseTime > 0)
//             ::SetTimer(m_hWnd, 0xec0e, m_nAutoCloseTime, NULL);// 0x3df32
//         m_pWndDlg->SetWindowPos(NULL, 0,0,0,0, 0x5f);          // 0x3df5f
//         ::ValidateRect(m_pWndDlg->m_hWnd, NULL);               // 0x3df71
//         if (afxGlobalData.<+0x288> > 8 && m_nTransparency < 255)
//             ::SetLayeredWindowAttributes(m_hWnd, 0, m_nTransparency, LWA_ALPHA);
//     }
//
// DEVIATION: the two step-size divisions are guarded against a zero
// m_FinalSize component (retail divides unguarded and would raise #DE);
// m_FinalSize is only zero on an object OpenMFC's placement-new constructor
// left zeroed, because retail's CommonCreate always fills it in first.
// The ShowWindow / SetWindowPos calls on m_pWndDlg and the buttons go through
// the non-virtual CWnd thunks, exactly the members retail calls directly
// (0x2a79e0 ?ShowWindow@CWnd, 0x2a7970 ?SetWindowPos@CWnd).
// DEVIATION: retail's m_pWndDlg->SetWindowPos(...0x5f) + ::ValidateRect pair
// (0x3df38..0x3df71) dereferences m_pWndDlg with NO null check; here it is
// skipped when m_pWndDlg is NULL, which is the state OpenMFC's stubbed Create
// leaves behind.
// Symbol: ?StartAnimation@CMFCDesktopAlertWnd@@IEAAXH@Z
extern "C" void MS_ABI impl__StartAnimation_CMFCDesktopAlertWnd__IEAAXH_Z(void* pThis, int bShow) {
    if (pThis == nullptr) return;
    AlertWndLayout* p = L(pThis);
    const HWND hWnd = HWndOf(pThis);

    const int type = ActualAnimationType(p);

    bool bAnimate = (type != kAnimNone);
    if (bAnimate) {
        EnsureGlobalData();
        if (GlobalDataInt(kGlobalDataIsRemoteSess) != 0) bAnimate = false;
    }
    if (bAnimate && type == kAnimFade) {
        EnsureGlobalData();
        if (GlobalDataInt(kGlobalDataBitsPerPixel) <= 8) bAnimate = false;
    }

    if (bAnimate) {
        p->m_bAnimationIsDone  = FALSE;
        p->m_bFadeOutAnimation = (bShow == 0) ? 1 : 0;

        RECT rectScreen = { 0, 0, 0, 0 };
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
        RECT rectWindow = { 0, 0, 0, 0 };
        ::GetWindowRect(hWnd, &rectWindow);

        p->m_bIsAnimRight = (rectWindow.left < (rectScreen.right + rectScreen.left) / 2) ? 1 : 0;
        p->m_bIsAnimDown  = (rectWindow.top  < (rectScreen.bottom + rectScreen.top) / 2) ? 1 : 0;

        p->m_iFadePercent = p->m_bFadeOutAnimation ? 100 : 0;
        p->m_iFadeStep    = p->m_bFadeOutAnimation ? -10 : 10;

        int nStepX;
        int nStepY;
        if (p->m_FinalSize.cx > p->m_FinalSize.cy) {
            nStepY = 10;
            nStepX = (p->m_FinalSize.cy != 0) ? (p->m_FinalSize.cx * 10) / p->m_FinalSize.cy : 1;
            if (nStepX < 1) nStepX = 1;
        } else {
            nStepX = 10;
            nStepY = (p->m_FinalSize.cx != 0) ? (p->m_FinalSize.cy * 10) / p->m_FinalSize.cx : 1;
            if (nStepY < 1) nStepY = 1;
        }
        p->m_nAnimStepX = nStepX;
        p->m_nAnimStepY = nStepY;

        p->m_AnimSize = p->m_FinalSize;
        if (bShow) {
            if (type == kAnimUnfold) {
                p->m_AnimSize.cx = nStepX;
                p->m_AnimSize.cy = nStepY;
            } else if (type == kAnimSlide) {
                p->m_AnimSize.cy = nStepY;
            }
        }

        if (p->m_pWndDlg != nullptr && ::IsWindowVisible(HWndOf(p->m_pWndDlg))) {
            impl__ShowWindow_CWnd__QEAAHH_Z(p->m_pWndDlg, SW_HIDE);
        }
        ::SetTimer(hWnd, kTimerAnimate, p->m_AnimationSpeed, nullptr);
        g_nLastAnimationTick = static_cast<int>(std::clock());
        return;
    }

    if (!bShow) {
        ::SendMessage(hWnd, WM_CLOSE, 0, 0);
        return;
    }

    if (HWndOf(p->m_btnClose) != nullptr) {
        impl__ShowWindow_CWnd__QEAAHH_Z(p->m_btnClose, SW_SHOWNOACTIVATE);   // edx = 4
    }
    if (HWndOf(p->m_btnMenu) != nullptr) {
        impl__ShowWindow_CWnd__QEAAHH_Z(p->m_btnMenu, SW_SHOWNOACTIVATE);   // edx = 4
    }
    p->m_bAnimationIsDone = TRUE;
    if (p->m_nAutoCloseTime > 0) {
        ::SetTimer(hWnd, kTimerAutoClose, static_cast<UINT>(p->m_nAutoCloseTime), nullptr);
    }
    if (p->m_pWndDlg != nullptr) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(p->m_pWndDlg, nullptr, 0, 0, 0, 0,
                                                    SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
                                                    SWP_NOREDRAW | SWP_NOACTIVATE | SWP_SHOWWINDOW); // 0x5f
        ::ValidateRect(HWndOf(p->m_pWndDlg), nullptr);
    }
    EnsureGlobalData();
    if (GlobalDataInt(kGlobalDataBitsPerPixel) > 8 && p->m_nTransparency < 0xff) {
        ::SetLayeredWindowAttributes(hWnd, 0, p->m_nTransparency, LWA_ALPHA);
    }
}

// Retail RVA 0x3d640 (mfc140), fully transcribed -- nine instructions:
//     if ((UINT)(nSpeed - 1) > 199) return;     // i.e. 1 <= nSpeed <= 200
//     m_AnimationSpeed = max(nSpeed, 2u);       // cmovb against 2
// Symbol: ?SetAnimationSpeed@CMFCDesktopAlertWnd@@QEAAXI@Z
extern "C" void MS_ABI impl__SetAnimationSpeed_CMFCDesktopAlertWnd__QEAAXI_Z(void* pThis, unsigned int nSpeed) {
    if (pThis == nullptr) return;
    if (nSpeed - 1u > 199u) return;
    L(pThis)->m_AnimationSpeed = (nSpeed < 2u) ? 2u : nSpeed;
}

// ===========================================================================
// Geometry
// ===========================================================================

// Retail RVA 0x3e640 (mfc140), fully transcribed:
//     if (m_bSmallCaption) return 7;
//     int nHeight = CMenuImages::Size().cy + 8;      // 0x8fbb0
//     if (::GetSystemMetrics(SM_CYSMSIZE) > nHeight)
//         nHeight = ::GetSystemMetrics(SM_CYSMSIZE); // 0x33 == SM_CYSMSIZE
//     return nHeight;
// Symbol: ?GetCaptionHeight@CMFCDesktopAlertWnd@@UEAAHXZ
extern "C" int MS_ABI impl__GetCaptionHeight_CMFCDesktopAlertWnd__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return 0;
    if (L(pThis)->m_bSmallCaption != 0) return 7;

    SIZE sizeImage = { 0, 0 };
    impl__Size_CMenuImages__SA_AVCSize__XZ(&sizeImage);
    int nHeight = sizeImage.cy + 8;
    const int nSysHeight = ::GetSystemMetrics(SM_CYSMSIZE);
    if (nSysHeight > nHeight) nHeight = nSysHeight;
    return nHeight;
}

// Retail RVA 0x3d660 (mfc140), fully transcribed:
//     int nHeight = GetCaptionHeight();          // vftable slot 96 (+0x300)
//     ::GetClientRect(m_hWnd, &rect);
//     *pRet = rect;
//     ::InflateRect(pRet, -1, -1);
//     pRet->bottom = pRet->top + nHeight;
//     return pRet;
// DEVIATION: retail dispatches GetCaptionHeight through the vftable so a
// derived class can override it; OpenMFC authors no MSVC vftable for this
// class, so the call goes straight to this file's thunk and an override in a
// client subclass is NOT honoured.
// Symbol: ?GetCaptionRect@CMFCDesktopAlertWnd@@IEAA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetCaptionRect_CMFCDesktopAlertWnd__IEAA_AVCRect__XZ(void* pThis, RECT* pRet) {
    if (pRet == nullptr) return nullptr;
    if (pThis == nullptr) {
        pRet->left = pRet->top = pRet->right = pRet->bottom = 0;
        return pRet;
    }
    const int nHeight = impl__GetCaptionHeight_CMFCDesktopAlertWnd__UEAAHXZ(pThis);
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(HWndOf(pThis), &rect);
    *pRet = rect;
    ::InflateRect(pRet, -1, -1);
    pRet->bottom = pRet->top + nHeight;
    return pRet;
}

// Retail RVA 0x3d070 (mfc140), fully transcribed:
//     CSize size(0, 0);
//     CDialogTemplate dlgTemplate;
//     if (!dlgTemplate.Load(MAKEINTRESOURCE(m_uiDlgResID)))   // 0x218490,
//         *pRet = CSize(0, 0);                                //   WORD-widened
//     else { CSize sz(0,0); dlgTemplate.GetSizeInPixels(&sz); *pRet = sz; }
//     ~CDialogTemplate: if (m_hTemplate) ::GlobalFree(m_hTemplate);
//     return pRet;
// The template object is 16 bytes of zeroed stack in retail (0x20(%rsp) with
// only two qwords initialised); afxpriv.h:708 declares it as
// { HGLOBAL m_hTemplate; DWORD m_dwTemplateSize; BOOL m_bSystemFont; }, so 16
// bytes covers it.  Both CDialogTemplate thunks are still null stubs in this
// tree (see `headerRequests`), so today every call returns CSize(0, 0) -- but
// through the retail control flow, and correctly by the sret ABI, which the
// generated stub this replaces got wrong (it returned nullptr instead of pRet).
// Symbol: ?GetDialogSize@CMFCDesktopAlertWnd@@UEAA?AVCSize@@XZ
extern "C" void* MS_ABI impl__GetDialogSize_CMFCDesktopAlertWnd__UEAA_AVCSize__XZ(void* pThis, SIZE* pRet) {
    if (pRet == nullptr) return nullptr;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;

    alignas(void*) unsigned char dlgTemplate[16] = {};
    const unsigned int uiDlgResID = L(pThis)->m_uiDlgResID;
    if (impl__Load_CDialogTemplate__QEAAHPEB_W_Z(
            dlgTemplate, MAKEINTRESOURCEW(static_cast<WORD>(uiDlgResID))) != 0) {
        SIZE size = { 0, 0 };
        impl__GetSizeInPixels_CDialogTemplate__QEBAXPEAUtagSIZE___Z(dlgTemplate, &size);
        *pRet = size;
    }
    HGLOBAL hTemplate;
    std::memcpy(&hTemplate, dlgTemplate, sizeof hTemplate);
    if (hTemplate != nullptr) ::GlobalFree(hTemplate);
    return pRet;
}

// ===========================================================================
// Painting
// ===========================================================================

// Retail RVA 0x3d960 (mfc140).  NOT transcribed -- see the `stubbed` report.
// The body reads ::GetClientRect, then dispatches twice on the visual
// manager: slot 122 (+0x3d0), which is
// ?OnDrawPopupWindowBorder@CMFCVisualManager@@UEAAXPEAVCDC@@VCRect@@@Z
// (RVA 0x189730, mfc140 -- read out of the vftable qword preceding the one
// below), and slot 123 (+0x3d8), which is
// ?OnDrawPopupWindowCaption@CMFCVisualManager@@UEAAKPEAVCDC@@VCRect@@PEAVCMFCDesktopAlertWnd@@@Z
// (RVA 0x189810, mfc140, the qword at VA 0x18031a350) and returns the caption
// text colour.  Then -- only when m_bSmallCaption is FALSE -- it moves the
// caption rect's LEFT edge in by 4, pulls its right edge in to 4 px left of
// the menu button (or, if that has no HWND, the close button) via
// ::GetWindowRect + CWnd::ScreenToClient (0x2a11f0), fetches the icon with
// ::SendMessage(m_hWnd, WM_GETICON, 0, 0), paints it with
// ::DrawState(pDC->m_hDC, NULL, NULL, (LPARAM)hIcon, 0, x, y, cx, cy,
// DST_ICON) at afxGlobalData.m_sizeSmallIcon (+0x1a0), and finally draws the
// window text with DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS (0x8024) in
// afxGlobalData.fontRegular (+0x1a8 -- the first CFont after m_sizeSmallIcon
// in afxglobals.h, NOT fontBold).  That last half needs CWnd::GetWindowText
// (0x28a280) into a CString, CDC::SetBkMode (0x2a07a0) and five CDC vftable
// dispatches at three slots (+0x70 twice, +0x60 twice, +0xe0 once, i.e.
// slots 14, 12 and 28 -- their MFC names are not verified here); this tree's
// CString-by-reference ABI and CDC vftable are not settled enough to
// transcribe it faithfully, and a half-painted caption is worse than none.
// Symbol: ?OnDraw@CMFCDesktopAlertWnd@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CMFCDesktopAlertWnd__UEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
}

// Retail: the WM_ERASEBKGND entry of the message map at 0x1802e20c0 (mfc140)
// points at RVA 0x3ae0, whose whole body is `mov $1,%eax; ret` -- the handler
// returns TRUE and erases nothing.  (0x3ae0 carries the name ?OnEraseBkgnd@CPaneTrackingWnd@@... in the
// symbol map: the linker folded the two identical bodies, which is also why
// this export has no RVA of its own in the map.)
// Symbol: ?OnEraseBkgnd@CMFCDesktopAlertWnd@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCDesktopAlertWnd__IEAAHPEAVCDC___Z(void* pThis, void* pDC) {
    (void)pThis; (void)pDC;
    return TRUE;
}

// Retail RVA 0x3d5c0 (mfc140), fully transcribed:
//     CPaintDC dc(this);                       // 0x2a1c60
//     if (!m_bAnimationIsDone) DrawAnimation(&dc);          // 0x3dfe0
//     else                     OnDraw(&dc);                 // vftable slot 95
//     ~CPaintDC();                             // 0x2a1d10
// CPaintDC exists in this DLL only as its impl__ thunks, so the object is
// built in raw storage and torn down through the matching thunk, exactly as
// featurepack/menu/CMFCPopupMenu.cpp:1301 does.
// DEVIATION: OnDraw is reached non-virtually (no MSVC vftable is authored for
// this class), so a client override is not honoured; both callees are still
// stubs in this file.
// Symbol: ?OnPaint@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    // OpenMFC's CPaintDC (include/openmfc/afxwin.h:2060 -- the 32-byte CDC
    // base plus PAINTSTRUCT m_ps plus CWnd* m_pWnd) measures 112 bytes today;
    // 256 zeroed bytes over-allocate it on purpose so this TU does not have to
    // include the class definition (see the note at the top of the file).
    alignas(16) unsigned char dcStorage[256] = {};
    void* pDC = dcStorage;
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, pThis);
    if (L(pThis)->m_bAnimationIsDone == 0) {
        impl__DrawAnimation_CMFCDesktopAlertWnd__IEAAXPEAVCDC___Z(pThis, pDC);
    } else {
        impl__OnDraw_CMFCDesktopAlertWnd__UEAAXPEAVCDC___Z(pThis, pDC);
    }
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// Retail RVA 0x3d930 (mfc140), fully transcribed -- eight instructions:
//     if (nFlags & PRF_CLIENT /*4*/) OnDraw(pDC);   // vftable slot 95 (+0x2f8)
//     return 0;
// DEVIATION: OnDraw non-virtual, as in OnPaint above.
// Symbol: ?OnPrintClient@CMFCDesktopAlertWnd@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CMFCDesktopAlertWnd__IEAA_JPEAVCDC__I_Z(
    void* pThis, void* pDC, unsigned int nFlags) {
    if (pThis != nullptr && (nFlags & PRF_CLIENT) != 0) {
        impl__OnDraw_CMFCDesktopAlertWnd__UEAAXPEAVCDC___Z(pThis, pDC);
    }
    return 0;
}

// ===========================================================================
// Lifetime
// ===========================================================================

// Retail RVA 0x3e690 (mfc140), fully transcribed:
//     if (m_pWndOwner != NULL && m_pWndOwner->m_hWnd != NULL)
//         ::SendMessage(m_pWndOwner->m_hWnd, AFX_WM_ON_CLOSEPOPUPWINDOW,
//                       0, (LPARAM)this);
//     CWnd::Default();                              // tail jump to 0x289090
// Symbol: ?OnClose@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pOwner = L(pThis)->m_pWndOwner;
    if (pOwner != nullptr) {
        HWND hOwner = HWndOf(pOwner);
        if (hOwner != nullptr) {
            ::SendMessage(hOwner, impl__AFX_WM_ON_CLOSEPOPUPWINDOW__3IA, 0,
                          reinterpret_cast<LPARAM>(pThis));
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Retail RVA 0x3d8d0 (mfc140), fully transcribed:
//     if (m_pWndDlg != NULL) {
//         m_pWndDlg->DestroyWindow();       // CWnd vftable slot 26 (+0xd0)
//         if (m_pWndDlg != NULL)
//             m_pWndDlg->`scalar deleting destructor'(1);  // slot 1 == delete
//         m_pWndDlg = NULL;
//     }
//     CWnd::OnDestroy();                    // tail jump to 0x289cc0
// The DestroyWindow call goes straight to the CWnd thunk: slot 26 of the
// retail CMFCDesktopAlertDialog vftable (0x1802e1d18 in mfc140, the vfptr its
// constructor 0x3b420 stores) is ?DestroyWindow@CWnd@@UEAAHXZ itself, so only
// a client-derived dialog overriding DestroyWindow would behave differently.
// DEVIATION: `delete m_pWndDlg` is not performed here.  Retail reaches the
// dialog's deleting destructor through its own MSVC vftable slot 1; OpenMFC
// authors no such vftable for CMFCDesktopAlertDialog, and its destructor thunk
// (featurepack/controls/CMFCDesktopAlertDialog.cpp:11) is a generated stub
// that takes no `this` and returns nullptr, freeing nothing, so calling it
// would only mis-report progress.
// Since OpenMFC's Create never allocates the dialog (both Create overloads are
// stubs), m_pWndDlg is always NULL today and nothing leaks; this becomes a
// real leak the moment CommonCreate is implemented -- flagged in the report.
// Symbol: ?OnDestroy@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AlertWndLayout* p = L(pThis);
    if (p->m_pWndDlg != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(p->m_pWndDlg);
        p->m_pWndDlg = nullptr;
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}

// Retail RVA 0x3d590 (mfc140), fully transcribed -- the classic self-deleting
// popup tail:
//     CWnd::OnNcDestroy();                        // 0x289d80
//     if (this != NULL) this->`scalar deleting destructor'(1);  // vftable
//                                                 // slot 1 (0x3c8a0), i.e.
//                                                 // `delete this'
// DEVIATION: `delete this' is not performed.  Retail reaches the scalar
// deleting destructor through slot 1 of the object's MSVC vftable.  Here the
// object was built by OpenMFC's placement-new constructor thunk
// (featurepack/controls/CtorDtorPlacement.cpp:24), so its vptr is the
// clean-room compiler's, not an MSVC vftable, and neither a slot-1 dispatch
// nor the client's matching deallocator can be reached safely from this TU
// -- the same reason featurepack/menu/CMFCPopupMenu.cpp's PostNcDestroy
// omits its `delete this`.  Consequence: a heap-allocated alert window
// (retail's intended use: `new CMFCDesktopAlertWnd` and never deleted by the
// caller) LEAKS its storage once destroyed.  The CWnd::OnNcDestroy half is
// performed.
// Symbol: ?OnNcDestroy@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcDestroy_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__OnNcDestroy_CWnd__IEAAXXZ(pThis);
}

// ===========================================================================
// Commands
// ===========================================================================

// Retail RVA 0x3d6f0 (mfc140), fully transcribed:
//     if (hwnd == m_btnClose.GetSafeHwnd()) {
//         ::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
//         return TRUE;
//     }
//     if (hwnd != m_btnMenu.GetSafeHwnd() || m_hMenu == NULL) return FALSE;
//     ::GetWindowRect(m_btnMenu.m_hWnd, &rectBtn);
//     m_pWndDlg-><+0xeb8> = 1;                    // "menu is tracking"
//     UINT nCmd;
//     if (afxContextMenuManager /* .data 0x3b6f10, mfc140 */ != NULL) {
//         int nSave = <global at 0x1803aab00>; <global at 0x1803aab00> = 0;
//         nCmd = pManager->vftable slot 7(m_hMenu, rectBtn.left,
//                                         rectBtn.bottom, this, FALSE);
//         <global at 0x1803aab00> = nSave;
//     } else {
//         nCmd = ::TrackPopupMenu(m_hMenu, TPM_NONOTIFY|TPM_RETURNCMD /*0x180*/,
//                                 rectBtn.left, rectBtn.bottom, 0, m_hWnd, NULL);
//     }
//     m_pWndDlg-><+0xeb8> = 0;
//     if (nCmd != 0)
//         ::PostMessage(m_pWndOwner ? m_pWndOwner->m_hWnd : m_pWndDlg->m_hWnd,
//                       WM_COMMAND, nCmd, 0);
//     OnCancelMode();                              // 0x3e940
//     return TRUE;
//
// DEVIATIONS, both because the pieces do not exist in this tree:
//  * the afxContextMenuManager branch is not taken.  That global is not an
//    export and OpenMFC keeps no equivalent -- the same gap
//    featurepack/ribbon/CMFCRibbonBar.cpp:3643 (at this same mfc140 address
//    0x3b6f10) and featurepack/menu/CMFCPopupMenuBar.cpp:870 (which cites the
//    global under a different address, 0x3be1b0) already document -- so this always takes the ::TrackPopupMenu fallback,
//    which is the branch retail itself takes when the manager is absent;
//  * the two `m_pWndDlg-><+0xeb8> = 1/0` writes around the menu are skipped
//    (the unidentified global at 0x1803aab00 is only touched on the skipped
//    context-menu-manager branch): +0xeb8 lies past OpenMFC's placeholder
//    CMFCDesktopAlertDialog, so there is nothing to write to.  In retail that
//    flag is read by CMFCDesktopAlertDialog::HasFocus (0x3b840, mfc140),
//    which returns TRUE while it is set -- so the menu being up keeps
//    OnTimer's 0xec10 arm reporting the alert active and holds off the
//    auto-close.  OpenMFC's HasFocus is a null stub returning 0, so skipping
//    the write loses nothing today; it must be restored together with
//    HasFocus.
// Symbol: ?ProcessCommand@CMFCDesktopAlertWnd@@QEAAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__ProcessCommand_CMFCDesktopAlertWnd__QEAAHPEAUHWND_____Z(void* pThis, HWND hwnd) {
    if (pThis == nullptr) return FALSE;
    AlertWndLayout* p = L(pThis);

    if (hwnd == HWndOf(p->m_btnClose)) {
        ::SendMessage(HWndOf(pThis), WM_CLOSE, 0, 0);
        return TRUE;
    }
    if (hwnd != HWndOf(p->m_btnMenu) || p->m_hMenu == nullptr) return FALSE;

    RECT rectBtn = { 0, 0, 0, 0 };
    ::GetWindowRect(HWndOf(p->m_btnMenu), &rectBtn);

    const UINT nCmd = static_cast<UINT>(::TrackPopupMenu(
        p->m_hMenu, TPM_NONOTIFY | TPM_RETURNCMD,
        rectBtn.left, rectBtn.bottom, 0, HWndOf(pThis), nullptr));

    if (nCmd != 0) {
        HWND hTarget = (p->m_pWndOwner != nullptr) ? HWndOf(p->m_pWndOwner)
                                                   : HWndOf(p->m_pWndDlg);
        ::PostMessage(hTarget, WM_COMMAND, nCmd, 0);
    }
    impl__OnCancelMode_CMFCDesktopAlertWnd__IEAAXXZ(pThis);
    return TRUE;
}

// Retail RVA 0x3d880 (mfc140), fully transcribed:
//     if (ProcessCommand((HWND)lParam)) return TRUE;      // 0x3d6f0
//     return CWnd::OnCommand(wParam, lParam);             // 0x28c4c0
// Symbol: ?OnCommand@CMFCDesktopAlertWnd@@UEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCDesktopAlertWnd__UEAAH_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return FALSE;
    if (impl__ProcessCommand_CMFCDesktopAlertWnd__QEAAHPEAUHWND_____Z(
            pThis, reinterpret_cast<HWND>(static_cast<LPARAM>(lParam))) != 0) {
        return TRUE;
    }
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis,
                                             static_cast<WPARAM>(wParam),
                                             static_cast<LPARAM>(lParam));
}

// ===========================================================================
// Caption drag
// ===========================================================================

// Retail RVA 0x3e6d0 (mfc140), fully transcribed:
//     m_bMoving = TRUE;
//     ::GetCursorPos(&m_ptStartMove);
//     CWnd::FromHandle(::SetCapture(m_hWnd));   // i.e. CWnd::SetCapture()
//     ::KillTimer(m_hWnd, 0xec0e);              // tail jump
// The FromHandle result is discarded in retail too (it is the return value of
// the inlined CWnd::SetCapture), so only ::SetCapture is reproduced here.
// Symbol: ?StartWindowMove@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__StartWindowMove_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AlertWndLayout* p = L(pThis);
    p->m_bMoving = TRUE;
    ::GetCursorPos(&p->m_ptStartMove);
    ::SetCapture(HWndOf(pThis));
    ::KillTimer(HWndOf(pThis), kTimerAutoClose);
}

// Retail RVA 0x3e9e0 (mfc140) is a single `jmp 0x3e6d0` -- the whole body is
// `StartWindowMove();`, with nFlags and point unused (and CWnd::Default is NOT
// called on this path).
// Symbol: ?OnLButtonDown@CMFCDesktopAlertWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCDesktopAlertWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags; (void)point;
    impl__StartWindowMove_CMFCDesktopAlertWnd__IEAAXXZ(pThis);
}

// Retail RVA 0x3e8a0 (mfc140), fully transcribed:
//     <the StopWindowMove tail>            // see the helper above
//     CWnd::Default();                     // 0x289090, on BOTH paths
// Symbol: ?OnLButtonUp@CMFCDesktopAlertWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCDesktopAlertWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags; (void)point;
    if (pThis == nullptr) return;
    StopWindowMove(L(pThis));
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Retail RVA 0x3e940 (mfc140), fully transcribed.  Note the order: unlike
// OnLButtonUp, CWnd::Default() runs FIRST here (0x3e95c), then the same
// StopWindowMove tail.
// Symbol: ?OnCancelMode@CMFCDesktopAlertWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCDesktopAlertWnd__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    StopWindowMove(L(pThis));
}

// Retail RVA 0x3e720 (mfc140), fully transcribed:
//     if (m_bMoving) {
//         CPoint pt = point;
//         ::ClientToScreen(m_hWnd, &pt);
//         MONITORINFO mi = { sizeof(mi) };
//         HMONITOR hm = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
//         if (::GetMonitorInfo(hm, &mi)) ::CopyRect(&rectScreen, &mi.rcWork);
//         else ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
//         CPoint ptCursor; ::GetCursorPos(&ptCursor);
//         int dx = ptCursor.x - m_ptStartMove.x;
//         int dy = ptCursor.y - m_ptStartMove.y;
//         m_ptStartMove = ptCursor;
//         ::GetWindowRect(m_hWnd, &rectWindow);
//         ::OffsetRect(&rectWindow, dx, dy);
//         if      (rectWindow.left  < rectScreen.left)
//             ::OffsetRect(&rectWindow, rectScreen.left - rectWindow.left, 0);
//         else if (rectWindow.right > rectScreen.right)
//             ::OffsetRect(&rectWindow, rectScreen.right - rectWindow.right, 0);
//         if      (rectWindow.top    < rectScreen.top)
//             ::OffsetRect(&rectWindow, 0, rectScreen.top - rectWindow.top);
//         else if (rectWindow.bottom > rectScreen.bottom)
//             ::OffsetRect(&rectWindow, 0, rectScreen.bottom - rectWindow.bottom);
//         SetWindowPos(NULL, rectWindow.left, rectWindow.top, -1, -1, 0x45);
//     }
//     CWnd::Default();                     // 0x3e87a -- runs on BOTH paths
// 0x45 == SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW.
// Symbol: ?OnMouseMove@CMFCDesktopAlertWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCDesktopAlertWnd__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    AlertWndLayout* p = L(pThis);
    const HWND hWnd = HWndOf(pThis);

    if (p->m_bMoving != 0) {
        POINT pt = PointFromArg(point);
        ::ClientToScreen(hWnd, &pt);

        RECT rectScreen = { 0, 0, 0, 0 };
        MONITORINFO mi;
        std::memset(&mi, 0, sizeof mi);
        mi.cbSize = sizeof(MONITORINFO);
        HMONITOR hMonitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
        if (::GetMonitorInfo(hMonitor, &mi)) {
            ::CopyRect(&rectScreen, &mi.rcWork);
        } else {
            ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
        }

        POINT ptCursor = { 0, 0 };
        ::GetCursorPos(&ptCursor);
        const int dx = ptCursor.x - p->m_ptStartMove.x;
        const int dy = ptCursor.y - p->m_ptStartMove.y;
        p->m_ptStartMove = ptCursor;

        RECT rectWindow = { 0, 0, 0, 0 };
        ::GetWindowRect(hWnd, &rectWindow);
        ::OffsetRect(&rectWindow, dx, dy);

        if (rectWindow.left < rectScreen.left) {
            ::OffsetRect(&rectWindow, rectScreen.left - rectWindow.left, 0);
        } else if (rectWindow.right > rectScreen.right) {
            ::OffsetRect(&rectWindow, rectScreen.right - rectWindow.right, 0);
        }
        if (rectWindow.top < rectScreen.top) {
            ::OffsetRect(&rectWindow, 0, rectScreen.top - rectWindow.top);
        } else if (rectWindow.bottom > rectScreen.bottom) {
            ::OffsetRect(&rectWindow, 0, rectScreen.bottom - rectWindow.bottom);
        }

        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            pThis, nullptr, rectWindow.left, rectWindow.top, -1, -1,
            SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);   // 0x45
    }

    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// ===========================================================================
// Messages
// ===========================================================================

// Retail: the WM_WINDOWPOSCHANGED entry of the message map at 0x1802e20c0
// (mfc140) points at RVA 0xdab0, whose whole body is `jmp 0x289090`, i.e.
// `CWnd::Default();`.  (0xdab0 carries the name ?VKeyToItem@CListBox@@... in
// the symbol map: the linker folded the two identical one-instruction bodies,
// which is why this export has no RVA of its own.)  The handler exists only so
// the message map entry has a target; lpwndpos is unused.
// Symbol: ?OnWindowPosChanged@CMFCDesktopAlertWnd@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanged_CMFCDesktopAlertWnd__IEAAXPEAUtagWINDOWPOS___Z(
    void* pThis, WINDOWPOS* lpwndpos) {
    (void)lpwndpos;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Retail RVA 0x3d0f0 (mfc140), fully transcribed.  Three timer ids, dispatched
// by the chain of subtractions at 0x3d185..0x3d1a0:
//
//   0xec0e (auto-close, 0x3d53f):
//       if (!m_bIsActive && !m_bMoving) {
//           ::KillTimer(m_hWnd, 0xec0e);
//           StartAnimation(FALSE);              // 0x3dc60
//       }
//       -- and then RETURNS: CWnd::Default() is NOT called on this path.
//
//   0xec10 (hover tracking, 0x3d1a6):
//       if (!m_bMoving) {
//           ::GetWindowRect(m_hWnd, &rect);
//           BOOL bWasActive = m_bIsActive;
//           ::GetCursorPos(&pt);
//           BOOL bActive = ::PtInRect(&rect, pt);
//           if (!bActive) bActive = m_pWndDlg->HasFocus();     // 0x3b840
//           m_bIsActive = bActive;
//           if (bActive != bWasActive && afxGlobalData.<+0x288> > 8
//               && m_nTransparency < 255)
//               ::SetLayeredWindowAttributes(m_hWnd, 0,
//                   m_bIsActive ? 255 : m_nTransparency, LWA_ALPHA);
//       }
//       CWnd::Default();
//
//   0xec0f (animation step, 0x3d272): the body below; see the inline comments.
//
//   anything else: CWnd::Default().
//
// Not deviations in this body, but limits of what it drives today:
// DrawAnimation (reached from OnPaint while m_bAnimationIsDone is FALSE) is
// still a stub, so ANY animation in progress steps its counters but paints
// nothing; CMFCDesktopAlertDialog::HasFocus is a null stub in this tree, so
// the hover test reduces to PtInRect.
// DEVIATIONS: the division by m_AnimationSpeed is guarded (see below), and the
// finish block's m_pWndDlg->SetWindowPos + ::ValidateRect pair, which retail
// runs with no null check (0x3d43a..0x3d46d), is skipped when m_pWndDlg is
// NULL.
// Symbol: ?OnTimer@CMFCDesktopAlertWnd@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCDesktopAlertWnd__IEAAX_K_Z(void* pThis, unsigned __int64 nIDEvent) {
    if (pThis == nullptr) return;
    AlertWndLayout* p = L(pThis);
    const HWND hWnd = HWndOf(pThis);

    // GetActualAnimationType() is hoisted above the dispatch in retail
    // (0x3d117..0x3d182) and used only by the 0xec0f arm.
    const int type = ActualAnimationType(p);

    if (nIDEvent == kTimerAutoClose) {
        if (p->m_bIsActive == 0 && p->m_bMoving == 0) {
            ::KillTimer(hWnd, kTimerAutoClose);
            impl__StartAnimation_CMFCDesktopAlertWnd__IEAAXH_Z(pThis, FALSE);
        }
        return;
    }

    if (nIDEvent == kTimerActive) {
        if (p->m_bMoving == 0) {
            RECT rect = { 0, 0, 0, 0 };
            ::GetWindowRect(hWnd, &rect);
            const int bWasActive = p->m_bIsActive;
            POINT pt = { 0, 0 };
            ::GetCursorPos(&pt);
            int bActive = ::PtInRect(&rect, pt) ? 1 : 0;
            if (bActive == 0) {
                bActive = impl__HasFocus_CMFCDesktopAlertDialog__QEBAHXZ(p->m_pWndDlg) ? 1 : 0;
            }
            p->m_bIsActive = bActive;
            if (bActive != bWasActive) {
                EnsureGlobalData();
                if (GlobalDataInt(kGlobalDataBitsPerPixel) > 8 && p->m_nTransparency < 0xff) {
                    const BYTE alpha = (p->m_bIsActive != 0) ? 0xff : p->m_nTransparency;
                    ::SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
                }
            }
        }
        impl__Default_CWnd__IEAA_JXZ(pThis);
        return;
    }

    if (nIDEvent != kTimerAnimate || p->m_bAnimationIsDone != 0) {
        impl__Default_CWnd__IEAA_JXZ(pThis);
        return;
    }

    // ---- 0xec0f: one animation step ------------------------------------
    // nSteps = (int)((float)(clock() - g_nLastAnimationTick)
    //                / (float)m_AnimationSpeed + 0.5);      // 0x3d27f..0x3d2be
    // if (m_bFadeOutAnimation) nSteps = -nSteps;            // 0x3d2c3
    // (retail divides by m_AnimationSpeed unguarded; SetAnimationSpeed clamps
    //  it to >= 2 and the constructor seeds it with 30, so 0 is only reachable
    //  on an object OpenMFC's placement-new constructor left zeroed -- guarded
    //  here rather than producing an INT_MIN step count.)
    const int nNow = static_cast<int>(std::clock());
    const unsigned int nSpeed = (p->m_AnimationSpeed != 0) ? p->m_AnimationSpeed : 1u;
    int nSteps = static_cast<int>(
        static_cast<double>(static_cast<float>(nNow - g_nLastAnimationTick) /
                            static_cast<float>(nSpeed)) + 0.5);
    if (p->m_bFadeOutAnimation != 0) nSteps = -nSteps;

    // UNFOLD steps both axes (it falls through into the SLIDE arm at
    // 0x3d30b); SLIDE steps cy only; FADE steps m_iFadePercent and clamps it
    // to 101 once it passes m_iFadeStep * nSteps + 100.
    if (type == kAnimUnfold) {
        p->m_AnimSize.cx += nSteps * p->m_nAnimStepX;
        p->m_AnimSize.cy += nSteps * p->m_nAnimStepY;
    } else if (type == kAnimSlide) {
        p->m_AnimSize.cy += nSteps * p->m_nAnimStepY;
    } else if (type == kAnimFade) {
        const int nStep = p->m_iFadeStep;
        p->m_iFadePercent += nStep;
        if (p->m_iFadePercent > nStep * nSteps + 100) p->m_iFadePercent = 101;
    }

    // Clamp each axis into [0, m_FinalSize.*]                // 0x3d318..0x3d36e
    int cx = p->m_AnimSize.cx;
    int cy = p->m_AnimSize.cy;
    const int cxFinal = p->m_FinalSize.cx;
    const int cyFinal = p->m_FinalSize.cy;
    cx = (cx < cxFinal) ? cx : cxFinal;
    if (cx < 0) cx = 0;
    p->m_AnimSize.cx = cx;
    cy = (cy < cyFinal) ? cy : cyFinal;
    if (cy < 0) cy = 0;
    p->m_AnimSize.cy = cy;

    bool bFinished = false;
    if (p->m_bFadeOutAnimation != 0 && p->m_bIsActive == 0) {   // 0x3d374
        if (cx == 0 || cy == 0 ||
            (type == kAnimFade && p->m_iFadePercent <= 0)) {    // 0x3d382..0x3d396
            ::SendMessage(hWnd, WM_CLOSE, 0, 0);                // 0x3d398
            return;                                             // no Default()
        }
        bFinished = (type == kAnimFade) ? (p->m_iFadePercent > 100)
                                        : ((cy >= cyFinal && cx >= cxFinal) ||
                                           (type == kAnimUnfold && cx >= cxFinal) ||
                                           p->m_bIsActive != 0);
    } else if (type == kAnimFade) {                             // 0x3d3b1 -> 0x3d3cf
        // FADE falls through from the >100 test at 0x3d3cf into the
        // m_bIsActive test at 0x3d3d8, exactly like the size types do.
        bFinished = (p->m_iFadePercent > 100) || p->m_bIsActive != 0;
    } else {                                                    // 0x3d3b6
        bFinished = (cy >= cyFinal && cx >= cxFinal) ||
                    (type == kAnimUnfold && cx >= cxFinal) ||
                    p->m_bIsActive != 0;
    }

    if (bFinished) {                                            // 0x3d3e5
        p->m_AnimSize = p->m_FinalSize;
        ::KillTimer(hWnd, kTimerAnimate);
        if (HWndOf(p->m_btnClose) != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(p->m_btnClose, SW_SHOWNOACTIVATE);   // edx = 4
        }
        if (HWndOf(p->m_btnMenu) != nullptr) {
            impl__ShowWindow_CWnd__QEAAHH_Z(p->m_btnMenu, SW_SHOWNOACTIVATE);   // edx = 4
        }
        if (p->m_pWndDlg != nullptr) {
            impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
                p->m_pWndDlg, nullptr, 0, 0, 0, 0,
                SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
                SWP_NOREDRAW | SWP_NOACTIVATE | SWP_SHOWWINDOW);   // 0x5f
            ::ValidateRect(HWndOf(p->m_pWndDlg), nullptr);
        }
        p->m_bAnimationIsDone = TRUE;
        if (p->m_nAutoCloseTime > 0) {
            ::SetTimer(hWnd, kTimerAutoClose, static_cast<UINT>(p->m_nAutoCloseTime), nullptr);
        }
    }

    // 0x3d496: retail repositions/resizes the window ONLY while
    // m_bFadeOutAnimation is set and the type is not FADE (the two tests at
    // 0x3d496 and 0x3d49f both branch to the invalidate arm otherwise) -- so a
    // SHOW unfold/slide animation only invalidates and repaints; it is the
    // hide animation that shrinks the frame.
    if (p->m_bFadeOutAnimation != 0 && type != kAnimFade) {
        RECT rect = { 0, 0, 0, 0 };
        ::GetWindowRect(hWnd, &rect);
        const int x = (p->m_bIsAnimRight != 0) ? rect.left  : (rect.right  - p->m_AnimSize.cx);
        const int y = (p->m_bIsAnimDown  != 0) ? rect.top   : (rect.bottom - p->m_AnimSize.cy);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            pThis, nullptr, x, y, p->m_AnimSize.cx, p->m_AnimSize.cy,
            SWP_NOZORDER | SWP_NOACTIVATE);                        // 0x14
    } else {
        ::InvalidateRect(hWnd, nullptr, TRUE);
        ::UpdateWindow(hWnd);
    }
    g_nLastAnimationTick = nNow;

    impl__Default_CWnd__IEAA_JXZ(pThis);
}
