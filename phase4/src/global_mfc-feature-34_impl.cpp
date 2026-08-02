// OpenMFC: CMFCButton (afxbutton.h) remaining exports, wave1.
//
// All 30 symbols in this file were transcribed from the retail
// mfc140u.dll (14.51.36231, x64).  CMFCButton is a CWnd-derived owner-drawn
// button (size 0xB20 = 2848 bytes); the retail bodies address its members by
// raw byte offsets, so this file mirrors the class layout with a local
// struct `S` (see below) instead of relying on the (minimal) header decl.
//
// Where a retail body is too deep to model faithfully (image-object internals,
// visual-manager globals, the global C++ "singleton" state at 0x1803c1620,
// CWnd message-dispatch tail calls at 0x18028ac80) the implementation is a
// conservative, type-correct approximation marked "TODO(clean-room)".

#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Class statics defined in feature_static_data.cpp (CMFCButton::m_bWinXPTheme).
extern "C" std::int32_t impl__m_bWinXPTheme_CMFCButton__1HA;
extern "C" std::int32_t impl__m_bWinXPThemeWasChecked_CMFCButton__1HA;

//=============================================================================
// CMFCButton layout (harvested from ??0CMFCButton@@QEAA@XZ and members).
// CMFCButton is 0xB20 (2848) bytes.  Only the fields used by the 30 functions
// below are named; every other byte is padding so the offsets stay exact.
//=============================================================================
extern "C" void* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);

namespace {

struct S {
    void*         vfptr;            // 0x000: CObject vtable
    unsigned long m_dwRef;          // 0x008: CCmdTarget refcount
    char          _pad0c[0x40 - 0x0c];
    HWND          m_hWnd;           // 0x040: CWnd::m_hWnd
    char          _pad48[0xe8 - 0x48];
    int           m_nStyle;         // 0x0e8: 0..3 (3 = borderless/icon-ish)
    int           m_nFlatStyle;     // 0x0ec: BS_LEFT/RIGHT/CENTER derived, ctor=2
    int           m_nImageOffset;   // 0x0f0: image offset flag
    int           m_bImageIsTiled;  // 0x0f4: BOOL
    int           m_bMenuFont;      // 0x0f8: menu-font mode BOOL
    int           m_bDrawFocus;     // 0x0fc: draw-focus gate BOOL (default 1)
    int           m_bDrawText;      // 0x100: draw-text BOOL (default 1)
    int           m_bNotifyCommand; // 0x104: =1 posts BN_CLICKED, =0 sends
    int           m_bMultiline;     // 0x108: BOOL
    int           m_bTransparent;   // 0x10c: BOOL
    int           m_bPushed;        // 0x110: BOOL
    int           m_bHover;         // 0x114: BOOL
    int           m_bPress;         // 0x118: BOOL
    int           m_bClickStarted;  // 0x11c: BOOL
    int           m_bCapture;       // 0x120: BOOL
    int           m_bHighlighted;   // 0x124: BOOL
    int           m_bChecked;       // 0x128: BOOL
    int           m_bRadioStyle;    // 0x12c: BOOL
    int           m_bCheckStyle;    // 0x130: BOOL
    int           m_bAutoToggle;    // 0x134: BOOL
    char          _pad138[0x13c - 0x138];
    int           m_bDrawFocusA;    // 0x13c: BOOL pair
    int           m_bDrawFocusB;    // 0x140: BOOL pair
    int           m_nTimerID;       // 0x144: int (0xec0d used in SetTimer)
    HICON         m_hIcon;          // 0x148: HICON (8 bytes on x64).  Retail
                                    //        header names this region
                                    //        m_sizeImage/m_sizePushOffset;
                                    //        only the offsets matter here.
    int           m_nIconSize;      // 0x150: int
    int           m_ptImageX;       // 0x154: int
    char          m_Images[6][0x198]; // 0x158: six 408-byte image objects
    int           m_nImageIndex;    // 0xae8: int (default -1)
    int           m_nCheckedImageIndex; // 0xaec: int (default -1)
    int           m_nState;         // 0xaf0: int
    char          _padaf4[0xaf8 - 0xaf4];
    void*         pHelperWnd;       // 0xaf8: tooltip helper CWnd*
    void*         pMenuFont;        // 0xb00: CString slot reused as HFONT ptr
    char          _padb08[0xb10 - 0xb08];
    int           m_nBorderColor;   // 0xb10: int (default -1)
    int           m_nTextColor;     // 0xb14: int (default -1)
    int           m_nFocusColor;    // 0xb18: int (default -1)
    char          _padb1c[0xb20 - 0xb1c];
};

static_assert(sizeof(S) == 0xb20, "CMFCButton must be 0xB20 bytes");
static_assert(offsetof(S, vfptr) == 0x00, "vfptr @0x00");
static_assert(offsetof(S, m_dwRef) == 0x08, "m_dwRef @0x08");
static_assert(offsetof(S, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(S, m_nStyle) == 0xe8, "m_nStyle @0xe8");
static_assert(offsetof(S, m_nFlatStyle) == 0xec, "m_nFlatStyle @0xec");
static_assert(offsetof(S, m_bCheckStyle) == 0x130, "m_bCheckStyle @0x130");
static_assert(offsetof(S, m_bAutoToggle) == 0x134, "m_bAutoToggle @0x134");
static_assert(offsetof(S, m_hIcon) == 0x148, "m_hIcon @0x148");
static_assert(offsetof(S, m_Images) == 0x158, "m_Images @0x158");
static_assert(offsetof(S, m_nImageIndex) == 0xae8, "m_nImageIndex @0xae8");
static_assert(offsetof(S, m_nFocusColor) == 0xb18, "m_nFocusColor @0xb18");

// CDC::m_hDC lives at offset 8 in the CDC object.
inline HDC hdc_of(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}

// CMFCButton::UncheckRadioButtonsInGroup retail (RVA 0x1efb0, internal):
// walks the WS_GROUP radio siblings, removes WS_TABSTOP from each check-style
// sibling and unchecks it, then restores WS_TABSTOP on `this`.  Modeled here
// as a private helper (that symbol is not part of this wave's export list).
void UncheckRadioButtonsInGroupImpl(S* s) {
    if (s == nullptr) return;
    HWND hwndThis = s->m_hWnd;
    if (hwndThis == nullptr) return;
    HWND hParent = ::GetParent(hwndThis);
    if (hParent == nullptr) return;
    if (!::IsWindowEnabled(hwndThis)) return;

    bool tabStopChanged = false;
    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent, FALSE);
        // The retail resolves the sibling through CWnd::FromHandlePermanent and
        // requires it to be a CMFCButton (IsKindOf(RUNTIME_CLASS(CMFCButton)))
        // with m_bCheckStyle set.  We approximate the CMFCButton test by
        // resolving the HWND through FromHandlePermanent and reading
        // m_bCheckStyle (offset 0x130) directly.
        if (hwndNext == nullptr || hwndNext == hwndThis) break;
        HWND hwndBtn = hwndNext;
        void* pObj = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndBtn);
        if (pObj != nullptr && *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x130) != 0) {
            LONG_PTR style = ::GetWindowLongPtrW(hwndBtn, GWL_STYLE);
            if ((style & WS_TABSTOP) != 0) {
                ::SetWindowLongPtrW(hwndBtn, GWL_STYLE, style & ~WS_TABSTOP);
                tabStopChanged = true;
            }
            if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) != 0) {
                *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) = 0;
                ::RedrawWindow(hwndBtn, nullptr, nullptr,
                               RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
            }
        }
        hwndCurrent = hwndBtn;
    }
    if (tabStopChanged) {
        LONG_PTR style = ::GetWindowLongPtrW(hwndThis, GWL_STYLE);
        ::SetWindowLongPtrW(hwndThis, GWL_STYLE, style | WS_TABSTOP);
    }
}

} // namespace

//=============================================================================
// Forward declarations of sibling exports used by DrawItem/OnDraw below.
//=============================================================================
extern "C" void MS_ABI impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState);
extern "C" void MS_ABI impl__OnDrawBorder_CMFCButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rect, unsigned int uiState);
extern "C" void MS_ABI impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient);
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect);

//=============================================================================
// 1. Trivial accessors / static flag
//=============================================================================

// OnEraseBkgnd always reports the background as erased (the button paints
// everything itself through the draw virtuals).
// Symbol: ?OnEraseBkgnd@CMFCButton@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCButton__IEAAHPEAVCDC___Z(
    void* /*pThis*/, void* /*pDC*/)
{
    return 1;
}

// OnGetFont returns the font handle stored in the CString slot at 0xb00.
// Symbol: ?OnGetFont@CMFCButton@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CMFCButton__IEAAPEAUHFONT____XZ(
    void* pThis)
{
    if (pThis == nullptr) return nullptr;
    return reinterpret_cast<S*>(pThis)->pMenuFont;
}

// GetImageHorzMargin: fixed 10-pixel horizontal margin.
// Symbol: ?GetImageHorzMargin@CMFCButton@@MEBAHXZ
extern "C" int MS_ABI impl__GetImageHorzMargin_CMFCButton__MEBAHXZ(
    void* /*pThis*/)
{
    return 10;
}

// GetVertMargin: fixed 5-pixel vertical margin.
// Symbol: ?GetVertMargin@CMFCButton@@MEBAHXZ
extern "C" int MS_ABI impl__GetVertMargin_CMFCButton__MEBAHXZ(
    void* /*pThis*/)
{
    return 5;
}

// OnGetCheck returns the check state only for radio/check buttons.
// Symbol: ?OnGetCheck@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnGetCheck_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long /*wParam*/, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_bRadioStyle == 0 && s->m_bCheckStyle == 0) return 0;
    return s->m_bChecked != 0 ? 1 : 0;
}

// EnableWindowsTheming(int): static member, stores into the class static
// m_bWinXPTheme (defined in feature_static_data.cpp).
// Symbol: ?EnableWindowsTheming@CMFCButton@@SAXH@Z
extern "C" void MS_ABI impl__EnableWindowsTheming_CMFCButton__SAXH_Z(
    int bEnable)
{
    impl__m_bWinXPTheme_CMFCButton__1HA = bEnable;
}

//=============================================================================
// 2. State mutators
//=============================================================================

// InitStyle(UINT): decodes the button style bits into the radio/check/toggle
// flags and the flat-style, and lazily probes whether WinXP theming is
// available (once).
// Symbol: ?InitStyle@CMFCButton@@IEAAXK@Z
extern "C" void MS_ABI impl__InitStyle_CMFCButton__IEAAXK_Z(
    void* pThis, unsigned long dwStyle)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);

    switch (dwStyle & 0xf) {
    case 0x2: // BS_CHECKBOX
        s->m_bRadioStyle = 1;
        break;
    case 0x3: // BS_AUTOCHECKBOX
        s->m_bAutoToggle = 1;
        break;
    case 0x4: // BS_RADIOBUTTON
        s->m_bAutoToggle = 1;
        s->m_bCheckStyle = 1;
        break;
    case 0x9: // BS_AUTORADIOBUTTON
        s->m_bAutoToggle = 1;
        s->m_bCheckStyle = 1;
        break;
    default:
        break;
    }

    if (s->m_bRadioStyle != 0 || s->m_bCheckStyle != 0) {
        switch (dwStyle & 0x300) {
        case 0x100: s->m_nFlatStyle = 0; break; // BS_LEFT
        case 0x200: s->m_nFlatStyle = 1; break; // BS_RIGHT
        case 0x300: s->m_nFlatStyle = 2; break; // BS_CENTER
        default: break;
        }
    }

    // One-time WinXP-theme availability probe (retail RVA 0x1cdf7 loads
    // "uxtheme"-style support via an internal helper; GetProcAddress probe
    // is the equivalent).
    // TODO(clean-room): approximated by probing IsAppThemed directly.
    if (impl__m_bWinXPThemeWasChecked_CMFCButton__1HA == 0) {
        if (impl__m_bWinXPTheme_CMFCButton__1HA == 0) {
            int themed = 0;
            HMODULE hUx = ::LoadLibraryW(L"uxtheme.dll");
            if (hUx != nullptr) {
                FARPROC pIsAppThemed = ::GetProcAddress(hUx, "IsAppThemed");
                if (pIsAppThemed != nullptr) {
                    typedef BOOL(WINAPI *IsAppThemedFn)();
                    themed = (reinterpret_cast<IsAppThemedFn>(pIsAppThemed)() != FALSE) ? 1 : 0;
                }
            }
            impl__m_bWinXPTheme_CMFCButton__1HA = themed;
        }
        impl__m_bWinXPThemeWasChecked_CMFCButton__1HA = 1;
    }
}

// EnableMenuFont(BOOL, BOOL): stores the shared menu font (or NULL when
// disabled) into the 0xb00 slot and redraws when requested.
// Symbol: ?EnableMenuFont@CMFCButton@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableMenuFont_CMFCButton__QEAAXHH_Z(
    void* pThis, int bEnable, int bRedraw)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (bEnable != 0) {
        // Retail stores the process-global MFC menu font (0x1803c17d0).
        // DEFAULT_GUI_FONT is the closest public equivalent.
        // TODO(clean-room): uses DEFAULT_GUI_FONT instead of the MFC global.
        s->pMenuFont = reinterpret_cast<void*>(::GetStockObject(DEFAULT_GUI_FONT));
    } else {
        s->pMenuFont = nullptr;
    }
    if (bRedraw != 0) {
        HWND hwnd = s->m_hWnd;
        if (hwnd != nullptr) {
            ::InvalidateRect(hwnd, nullptr, TRUE);
            ::UpdateWindow(hwnd);
        }
    }
}

// OnSetCheck(WPARAM, LPARAM): mirrors OnGetCheck; updates m_bChecked when the
// requested state differs from the current one and unchecks the radio group.
// Symbol: ?OnSetCheck@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnSetCheck_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_bRadioStyle == 0 && s->m_bCheckStyle == 0) return 0;

    const int bNewCheck = (wParam != 0) ? 1 : 0;
    if ((bNewCheck == 0) == (s->m_bChecked == 0)) {
        return 0; // state unchanged
    }
    s->m_bChecked = bNewCheck;
    if (s->m_bCheckStyle != 0) {
        UncheckRadioButtonsInGroupImpl(s);
    }
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    return 0;
}

// OnGetImage(WPARAM, LPARAM): wParam==0 returns the first image object's
// bitmap cell (0x1f8); wParam==1 returns the icon from the image object.
// Symbol: ?OnGetImage@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnGetImage_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    if (wParam == 0) {
        // m_Images[0] + 0xa0 holds the bitmap handle.
        return static_cast<long long>(reinterpret_cast<long long>(
            *reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0x1f8)));
    }
    if (wParam == 1) {
        // Retail: m_Images[0].GetImage(0) -> HICON.  Image objects are not
        // modeled in this codebase.
        // TODO(clean-room): image-object GetImage not modeled.
        return 0;
    }
    return 0;
}

// CleanUp(): virtual cleanup -- resets the icon, the image indices and the six
// 408-byte image objects.
// Symbol: ?CleanUp@CMFCButton@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCButton__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->m_hIcon = nullptr;
    s->m_nImageIndex = -1;
    s->m_nCheckedImageIndex = -1;
    // Retail runs the image-object reset ctor (0x18016f690) on all six.
    // TODO(clean-room): image objects are zeroed instead of reconstructed.
    for (int i = 0; i < 6; ++i) {
        std::memset(s->m_Images[i], 0, 0x198);
    }
}

// ClearImages(BOOL): like CleanUp but clears the first three image objects
// (or the last three when bClearImages == FALSE) plus the icon state.
// Symbol: ?ClearImages@CMFCButton@@IEAAXH@Z
extern "C" void MS_ABI impl__ClearImages_CMFCButton__IEAAXH_Z(
    void* pThis, int bClearImages)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->m_hIcon = nullptr;
    s->m_nImageIndex = -1;
    s->m_nCheckedImageIndex = -1;
    // TODO(clean-room): image objects are zeroed instead of reconstructed.
    const int first = (bClearImages != 0) ? 0 : 3;
    for (int i = 0; i < 3; ++i) {
        std::memset(s->m_Images[first + i], 0, 0x198);
    }
}

//=============================================================================
// 3. Mouse / focus handlers
//=============================================================================

// OnMouseMove(UINT, CPoint): hot-tracking -- highlights the button while the
// pointer is inside and the left button is down, captures the mouse, and
// clears the states when the pointer leaves.
// Symbol: ?OnMouseMove@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->m_bHighlighted = 0;

    const bool bLeftDown = (nFlags & MK_LBUTTON) != 0;
    const bool bNeedTracking =
        bLeftDown || s->m_nStyle != 0 || impl__m_bWinXPTheme_CMFCButton__1HA != 0 ||
        s->m_bTransparent != 0;
    if (!bNeedTracking) {
        return; // retail tail-calls the CWnd base dispatch here
    }
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    RECT clientRect;
    std::memset(&clientRect, 0, sizeof(clientRect));
    ::GetClientRect(hwnd, &clientRect);

    POINT ptScreen = point;
    ::ClientToScreen(hwnd, &ptScreen);

    bool changed = false;
    if (::PtInRect(&clientRect, point)) {
        HWND hwndUnder = ::WindowFromPoint(ptScreen);
        void* pUnder = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndUnder);
        HWND hwndOf = (pUnder != nullptr)
                          ? *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pUnder) + 0x40)
                          : nullptr;
        if (hwndOf != hwnd) {
            goto not_over_button;
        }
        s->m_bHighlighted = 1;
        if (s->m_bClickStarted == 0) {
            s->m_bClickStarted = 1;
            changed = true;
        }
        if (bLeftDown) {
            if (s->m_bHover == 0 && s->m_bPress != 0) {
                s->m_bHover = 1;
                changed = true;
            }
        }
        if (s->m_bCapture == 0) {
            ::SetCapture(hwnd);
            s->m_bCapture = 1;
            changed = true;
        }
    } else {
not_over_button:
        if (bLeftDown) {
            if (s->m_bHover != 0) {
                s->m_bHover = 0;
                changed = true;
            }
        } else {
            if (s->m_bClickStarted != 0) {
                s->m_bClickStarted = 0;
                changed = true;
            }
        }
        if (s->m_bCapture != 0 && !bLeftDown) {
            ::ReleaseCapture();
            s->m_bCapture = 0;
        }
    }
    if (changed) {
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
    }
    // retail tail-calls the CWnd base dispatch here
}

// OnLButtonDown(UINT, CPoint): for multiline buttons forwards the press to the
// parent as WM_COMMAND; otherwise records pressed state, captures the mouse and
// starts the autorepeat timer.
// Symbol: ?OnLButtonDown@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    if (s->m_bMultiline != 0) {
        HWND hParent = ::GetParent(hwnd);
        if (hParent != nullptr) {
            WORD id = static_cast<WORD>(::GetDlgCtrlID(hwnd));
            ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, 0), reinterpret_cast<LPARAM>(hwnd));
        }
    } else {
        s->m_bHover = 1;
        s->m_bPress = 1;
        s->m_bClickStarted = 1;
        if (s->m_bCapture == 0) {
            ::SetCapture(hwnd);
            s->m_bCapture = 1;
        }
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
        if (s->m_nTimerID > 0) {
            ::SetTimer(hwnd, 0xec0d, static_cast<UINT>(s->m_nTimerID), nullptr);
        }
    }
    // retail tail-calls the CWnd base dispatch here
}

// OnLButtonUp(UINT, CPoint): completes the click -- toggles radio/check state,
// sends BN_CLICKED, releases the capture, kills the autorepeat timer and pops
// the tooltip.
// Symbol: ?OnLButtonUp@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    const bool bClickedInside =
        s->m_bHover != 0 && s->m_bPress != 0 && s->m_bClickStarted != 0;
    s->m_bHover = 0;
    s->m_bPress = 0;
    s->m_bClickStarted = 0;

    if (bClickedInside) {
        if (s->m_bAutoToggle != 0) {
            if (s->m_bRadioStyle != 0) {
                s->m_bChecked = (s->m_bChecked == 0) ? 1 : 0; // toggle
            } else if (s->m_bCheckStyle != 0 && s->m_bChecked == 0) {
                s->m_bChecked = 1;
                UncheckRadioButtonsInGroupImpl(s);
            }
        }
    }

    if (s->m_bPushed != 0) {
        s->m_bPushed = 0;
        HWND hParent = ::GetParent(hwnd);
        if (hParent != nullptr) {
            WORD id = static_cast<WORD>(::GetDlgCtrlID(hwnd));
            ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED),
                           reinterpret_cast<LPARAM>(hwnd));
        }
    }

    if (::IsWindow(hwnd)) {
        ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        // retail tail-calls the CWnd base dispatch here
        if (::IsWindow(hwnd)) {
            if (s->m_bCapture != 0) {
                ::ReleaseCapture();
                s->m_bCapture = 0;
            }
            if (s->m_nTimerID > 0) {
                ::KillTimer(hwnd, 0xec0d);
            }
            if (s->pHelperWnd != nullptr) {
                HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
                if (hwndHelper != nullptr) {
                    ::SendMessageW(hwndHelper, 0x41c, 0, 0); // TTM_POP
                }
            }
        }
    }
}

// OnLButtonDblClk(UINT, CPoint): treats a double-click like a press (sets the
// pressed/hover/click states, redraws) and marks the button as pushed.
// Symbol: ?OnLButtonDblClk@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->m_bHover = 1;
    s->m_bPress = 1;
    s->m_bClickStarted = 1;
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    // retail tail-calls the CWnd base dispatch here
    s->m_bPushed = 1;
}

// OnCancelMode(): releases the capture (if held), resets the pressed/hover
// states, redraws and kills the autorepeat timer.
// Symbol: ?OnCancelMode@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    // retail first tail-calls the CWnd base dispatch
    if (s->m_bCapture != 0) {
        ::ReleaseCapture();
        s->m_bClickStarted = 0;
        s->m_bCapture = 0;
        s->m_bHover = 0;
        s->m_bPress = 0;
        s->m_bHighlighted = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
    }
    if (s->m_nTimerID >= 0) {
        ::KillTimer(s->m_hWnd, 0xec0d);
    }
}

// OnKillFocus(CWnd*): releases the capture, resets the pressed/hover states and
// redraws.
// Symbol: ?OnKillFocus@CMFCButton@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCButton__IEAAXPEAVCWnd___Z(
    void* pThis, void* /*pNewWnd*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    // retail first tail-calls the CWnd base dispatch
    if (s->m_bCapture != 0) {
        ::ReleaseCapture();
        s->m_bCapture = 0;
    }
    s->m_bHover = 0;
    s->m_bPress = 0;
    s->m_bClickStarted = 0;
    s->m_bHighlighted = 0;
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}

// OnEnable(BOOL): when disabling, resets the pressed states and capture; always
// redraws.
// Symbol: ?OnEnable@CMFCButton@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCButton__IEAAXH_Z(
    void* pThis, int bEnable)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (bEnable == 0) {
        s->m_bHover = 0;
        s->m_bPress = 0;
        s->m_bClickStarted = 0;
        if (s->m_bCapture != 0) {
            ::ReleaseCapture();
            s->m_bCapture = 0;
        }
    }
    if (s->m_hWnd != nullptr) {
        ::RedrawWindow(s->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    // retail tail-calls the CWnd base dispatch here
}

// CheckNextPrevRadioButton(BOOL): walks the radio group with
// GetNextDlgGroupItem, finds the next visible/enabled check-style sibling,
// checks it and notifies the parent.
// Symbol: ?CheckNextPrevRadioButton@CMFCButton@@IEAAHH@Z
extern "C" int MS_ABI impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(
    void* pThis, int bNext)
{
    if (pThis == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_bCheckStyle == 0) return 0;
    HWND hwndThis = s->m_hWnd;
    if (hwndThis == nullptr) return 0;
    HWND hParent = ::GetParent(hwndThis);

    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent,
                                              (bNext != 0) ? FALSE : TRUE);
        if (hwndNext == nullptr || hwndNext == hwndThis) return 0;
        void* pNext = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndNext);
        if (pNext == pThis) return 0;
        if (pNext != nullptr &&
            *reinterpret_cast<int*>(reinterpret_cast<char*>(pNext) + 0x130) != 0) {
            // A check-style CMFCButton sibling; require it to be visible and
            // enabled (retail: (style & 0x18000000) == WS_VISIBLE).
            LONG_PTR style = ::GetWindowLongPtrW(hwndNext, GWL_STYLE);
            if ((style & 0x18000000L) == WS_VISIBLE) {
                if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pNext) + 0x128) != 0) {
                    return 0; // already checked
                }
                ::SendMessageW(hwndNext, BM_SETCHECK, BST_CHECKED, 1);
                WORD id = static_cast<WORD>(::GetWindowLongPtrW(hwndNext, GWL_ID));
                // Retail resolves the parent CWnd via FromHandlePermanent and
                // sends WM_COMMAND to its m_hWnd (== hParent).
                ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED),
                               reinterpret_cast<LPARAM>(hwndNext));
                return 1;
            }
        }
        hwndCurrent = hwndNext;
    }
}

//=============================================================================
// 4. Drawing virtuals
//=============================================================================

// OnDrawFocusRect(CDC*, const CRect&): deflates the rect, fills it with the
// focus color (unless themed/transparent) and draws the focus rectangle.
// Symbol: ?OnDrawFocusRect@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;

    RECT r = *rect;
    ::InflateRect(&r, -1, -1);

    COLORREF clrFocus = (s->m_nFocusColor != -1)
                            ? static_cast<COLORREF>(s->m_nFocusColor)
                            : static_cast<COLORREF>(::GetSysColor(COLOR_BTNFACE));
    // TODO(clean-room): retail reads the color from the global state singleton
    // (0x1803c1648) when m_nFocusColor == -1.

    const bool bThemedTransparent =
        impl__m_bWinXPTheme_CMFCButton__1HA != 0 && s->m_bTransparent != 0;
    if (!bThemedTransparent) {
        ::InflateRect(&r, -1, -1);
        HBRUSH hbr = ::CreateSolidBrush(clrFocus);
        ::FillRect(hdc, &r, hbr);
        ::DeleteObject(hbr);
    }
    ::DrawFocusRect(hdc, &r);
}

// OnDrawParentBackground(CDC*, CRect): fills the rect with the parent/button
// face background.  Retail forwards to the global visual-manager helper; that
// global is not modeled, so the button-face brush is used instead.
// Symbol: ?OnDrawParentBackground@CMFCButton@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawParentBackground_CMFCButton__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, void* pDC, RECT rect)
{
    (void)pThis;
    if (pDC == nullptr) return;
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;
    // TODO(clean-room): global visual-manager OnDrawParentBackground not modeled.
    ::FillRect(hdc, &rect, ::GetSysColorBrush(COLOR_BTNFACE));
}

// OnFillBackground(CDC*, const CRect&): fills the button face; when checked,
// draws a highlight frame around the interior.
// Symbol: ?OnFillBackground@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;

    if (s->m_bMenuFont != 0) {
        // Retail forwards to the global visual-manager parent-background
        // helper (0x18006bbd0); not modeled here.
        // TODO(clean-room): menu-font parent background not modeled.
        ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
    } else {
        // Retail fills with a cached brush from the global state; button face
        // is the closest public approximation.
        // TODO(clean-room): uses COLOR_BTNFACE instead of the global color.
        ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
    }

    if (s->m_bChecked != 0 && s->m_bDrawText != 0) {
        if (!(s->m_bHover != 0 && s->m_bClickStarted != 0)) {
            // Retail draws a highlight frame via 0x180056900; approximate with
            // a one-pixel highlight border.
            // TODO(clean-room): highlight frame approximated with FrameRect.
            RECT frame = *rectClient;
            ::InflateRect(&frame, -1, -1);
            ::FrameRect(hdc, &frame, ::GetSysColorBrush(COLOR_HIGHLIGHT));
        }
    }
}

// DrawBorder(CDC*, CRect&, UINT): draws the border and deflates rectClient by
// the space the border consumed.  Retail fills the face with global state
// colors; approximated with standard edge drawing.
// Symbol: ?DrawBorder@CMFCButton@@IEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__DrawBorder_CMFCButton__IEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;

    if (s->m_nStyle == 3) {
        // Borderless style: retail only deflates the rect.
        ::InflateRect(rectClient, -2, -2);
        return;
    }

    const bool bPressed = (s->m_bClickStarted != 0) || ((uiState & 0x1) != 0);
    const bool bHot = s->m_bHover != 0;

    // TODO(clean-room): retail fills the face with global-state colors and uses
    // finer pressed/hover distinctions; DrawEdge approximates the look.
    if (s->m_bChecked != 0) {
        ::DrawEdge(hdc, rectClient, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else if (bPressed) {
        ::DrawEdge(hdc, rectClient, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else if (bHot) {
        ::DrawEdge(hdc, rectClient, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else {
        ::DrawEdge(hdc, rectClient, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    }
}

// OnDrawBorder(CDC*, CRect&, UINT): retail is a direct jump to DrawBorder
// (RVA 0x1ec30 -> 0x1f160); forwarded to the same implementation.
// Symbol: ?OnDrawBorder@CMFCButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, unsigned int uiState)
{
    impl__DrawBorder_CMFCButton__IEAAXPEAVCDC__AEAVCRect__I_Z(
        pThis, pDC, rectClient, uiState);
}

// OnDrawText(CDC*, const CRect&, const CString&, UINT, UINT): draws the button
// text centred with the supplied format.  (uiState is unused in the retail
// body; the mangled name ends `II` = two UINT params.)
// Symbol: ?OnDrawText@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@II@Z
extern "C" void MS_ABI impl__OnDrawText_CMFCButton__MEAAXPEAVCDC__AEBVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__II_Z(
    void* pThis, void* pDC, const RECT* rect, const void* strText,
    unsigned int uiFormat, unsigned int /*uiState*/)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr || strText == nullptr) return;
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;
    const wchar_t* psz = *reinterpret_cast<const wchar_t* const*>(strText);
    if (psz == nullptr) return;
    // ATL CStringData: nDataLength precedes the buffer by 16 bytes.
    const int nLen = *reinterpret_cast<const int*>(
        reinterpret_cast<const char*>(psz) - 0x10);
    RECT r = *rect;
    ::DrawTextW(hdc, psz, nLen, &r, uiFormat);
}

// OnDraw(CDC*, const CRect&, UINT): main draw -- fills the background and draws
// the window text.  The image/icon and hot-text rendering paths of the retail
// body (RVA 0x1cf60) require the unmodeled image objects and visual-manager
// globals, so they are not reproduced.
// Symbol: ?OnDraw@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;

    impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
        pThis, pDC, rect);

    if (s->m_bDrawText != 0) {
        wchar_t buf[1024];
        int nLen = ::GetWindowTextW(s->m_hWnd, buf, 1024);
        if (nLen > 0) {
            RECT r = *rect;
            UINT format = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
            if (s->m_nFlatStyle == 1) format = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
            else if (s->m_nFlatStyle == 2) format = DT_SINGLELINE | DT_RIGHT | DT_VCENTER;
            // TODO(clean-room): image/icon + hot-text color rendering not modeled.
            ::SetTextColor(hdc, ::GetSysColor(COLOR_BTNTEXT));
            ::DrawTextW(hdc, buf, nLen, &r, format);
        }
    }
}

//=============================================================================
// 5. Owner-draw / control lifecycle
//=============================================================================

// DrawItem(LPDRAWITEMSTRUCT): owner-draw entry point -- validates the control
// type, then invokes the drawing virtuals in the same order as retail
// (OnDraw, OnDrawBorder, OnFillBackground, OnDrawFocusRect).
// Symbol: ?DrawItem@CMFCButton@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CMFCButton__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS)
{
    if (pThis == nullptr) return;
    if (lpDIS == nullptr || lpDIS->CtlType != ODT_BUTTON) {
        // Retail asserts (0x180227720) on a bad DRAWITEMSTRUCT.
        return;
    }
    // CDC::FromHandle equivalent: a minimal CDC head wrapping the HDC.
    struct CDCShim {
        void* vfptr;
        HDC   hDC;
        HDC   hAttribDC;
    } dc = { nullptr, lpDIS->hDC, lpDIS->hDC };
    void* pDC = &dc;

    RECT rect = lpDIS->rcItem;
    const unsigned int itemState = static_cast<unsigned int>(lpDIS->itemState);

    impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(pThis, pDC, &rect, itemState);
    impl__OnDrawBorder_CMFCButton__MEAAXPEAVCDC__AEAVCRect__I_Z(pThis, pDC, &rect, itemState);
    impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(pThis, pDC, &rect);

    if ((itemState & ODS_FOCUS) != 0) {
        S* s = reinterpret_cast<S*>(pThis);
        if (s->m_bDrawFocus != 0) {
            impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
                pThis, pDC, &rect);
        }
    }
}

// OnInitControl(WPARAM, LPARAM): retail parses the "MFCButton_Style",
// "MFCButton_Autosize", "MFCButton_Tooltip" and "MFCButton_FullTextTool"
// window properties and the runtime font to configure the button.  Those
// property/registry lookups and the font helper are not modeled.
// Symbol: ?OnInitControl@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnInitControl_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long /*wParam*/, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): property/style parsing not modeled.
    return 0;
}

// OnDestroy(): cleans up the tooltip helper object at 0xaf8.
// Symbol: ?OnDestroy@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr) {
            ::DestroyWindow(hwndHelper);
        }
        // Retail frees the helper object through 0x1801824a0; null it out.
        // TODO(clean-room): helper object is not deallocated.
        s->pHelperWnd = nullptr;
    }
    // retail tail-calls the CWnd base OnDestroy handler here
}
