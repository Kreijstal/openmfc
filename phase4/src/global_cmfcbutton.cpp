// OpenMFC: CMFCButton exports.
//
// All 26 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// CMFCButton is a CWnd-derived owner-drawn button (0xB20 = 2848 bytes) whose
// retail bodies address members by raw byte offsets.  The afxmfc.h declaration
// carries only public methods plus opaque padding, so the class is accessed
// through the file-local layout mirror below (offsets cross-checked against
// global_mfc-feature-34_impl.cpp, which implements the rest of the class).
//
// The image setter family (SetImage / SetCheckedImage / SetImageInternal) is
// routed to the three SetImageInternal overloads (retail RVAs 0x1d6f0, 0x1da00,
// 0x1dd00).  The retail overloads drive six unmodeled 408-byte image objects
// (m_Images at 0x158); the observable subset kept here is the primary handle
// slot at 0x1f8 (read back by OnGetImage) plus the icon size at 0x148/0x14c
// (read back by SizeToContent).  The internal CWnd dispatch tail calls
// (0x18028ac80 / 0x18028bc00 / 0x18028b640) and the image-object /
// visual-manager globals are not modeled and are marked "TODO(clean-room)".
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

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oaidl.h>

#include <cstddef>
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
extern "C" void* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__InitStyle_CMFCButton__IEAAXK_Z(void* pThis, unsigned long dwStyle);
extern "C" int MS_ABI impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(void* pThis, int bNext);
extern "C" int MS_ABI impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(
    HWND p0, unsigned long p1, unsigned long p2, unsigned int p3);
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
    void* pThis, void* hBitmap, int bAutoDestroy, void* hBitmapHot,
    int bAutoDestroyHot, int bChecked, void* hBitmapChecked);
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
    void* pThis, void* hIcon, int bAutoDestroy, void* hIconHot,
    int bChecked, void* hIconChecked, int bAutoDestroyHot);

namespace {

struct S {
    void*         vfptr;            // 0x000: CObject vtable
    unsigned long m_dwRef;          // 0x008: CCmdTarget refcount
    char          _pad0c[0x40 - 0x0c];
    HWND          m_hWnd;           // 0x040: CWnd::m_hWnd
    char          _pad48[0xe8 - 0x48];
    int           m_nStyle;         // 0x0e8: 0..3
    int           m_nFlatStyle;     // 0x0ec
    int           m_nImageOffset;   // 0x0f0
    int           m_bImageIsTiled;  // 0x0f4: BOOL
    int           m_bMenuFont;      // 0x0f8: BOOL
    int           m_bDrawFocus;     // 0x0fc: BOOL
    int           m_bDrawText;      // 0x100: BOOL
    int           m_bNotifyCommand; // 0x104: BOOL
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
    // Retail names this 8-byte region m_sizeImage (CSize), written by
    // SetImageInternal / SetStdImage and read by SizeToContent.
    int           m_sizeImageCX;    // 0x148
    int           m_sizeImageCY;    // 0x14c
    int           m_sizePushOffsetX;// 0x150 (m_sizePushOffset.cx)
    int           m_sizePushOffsetY;// 0x154 (m_sizePushOffset.cy)
    char          m_Images[6][0x198]; // 0x158: six 408-byte image objects
    int           m_nImageIndex;    // 0xae8: int (default -1)
    int           m_nCheckedImageIndex; // 0xaec: int (default -1)
    int           m_nState;         // 0xaf0: int
    char          _padaf4[0xaf8 - 0xaf4];
    void*         pHelperWnd;       // 0xaf8: tooltip helper CWnd*
    void*         pMenuFont;        // 0xb00: CString slot reused as HFONT ptr
    HICON         m_hCursor;        // 0xb08: custom cursor (OnSetCursor)
    int           m_nBorderColor;   // 0xb10: int (default -1)
    int           m_nTextColor;     // 0xb14: int (default -1)
    int           m_nFocusColor;    // 0xb18: int (default -1)
    char          _padb1c[0xb20 - 0xb1c];
};

static_assert(sizeof(S) == 0xb20, "CMFCButton must be 0xB20 bytes");
static_assert(offsetof(S, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(S, m_bHover) == 0x114, "m_bHover @0x114");
static_assert(offsetof(S, m_bClickStarted) == 0x11c, "m_bClickStarted @0x11c");
static_assert(offsetof(S, m_bChecked) == 0x128, "m_bChecked @0x128");
static_assert(offsetof(S, m_bRadioStyle) == 0x12c, "m_bRadioStyle @0x12c");
static_assert(offsetof(S, m_bCheckStyle) == 0x130, "m_bCheckStyle @0x130");
static_assert(offsetof(S, m_bAutoToggle) == 0x134, "m_bAutoToggle @0x134");
static_assert(offsetof(S, m_bDrawFocusA) == 0x13c, "m_bDrawFocusA @0x13c");
static_assert(offsetof(S, m_bDrawFocusB) == 0x140, "m_bDrawFocusB @0x140");
static_assert(offsetof(S, m_nTimerID) == 0x144, "m_nTimerID @0x144");
static_assert(offsetof(S, m_sizeImageCX) == 0x148, "m_sizeImage @0x148");
static_assert(offsetof(S, m_Images) == 0x158, "m_Images @0x158");
static_assert(offsetof(S, m_nImageIndex) == 0xae8, "m_nImageIndex @0xae8");
static_assert(offsetof(S, pHelperWnd) == 0xaf8, "pHelperWnd @0xaf8");
static_assert(offsetof(S, m_hCursor) == 0xb08, "m_hCursor @0xb08");

// CDC::m_hDC lives at offset 8 in the CDC object.
inline HDC hdc_of(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}

// First image object's handle slot (m_Images[0] + 0xa0); read back by the
// feature-34 OnGetImage export for wParam == 0.
inline void*& ImageHandleSlot(S* s) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(s) + 0x1f8);
}

// CMFCButton::UncheckRadioButtonsInGroup retail (RVA 0x1efb0): walks the
// radio siblings, removes WS_TABSTOP from each check-style sibling and
// unchecks it, then restores WS_TABSTOP on `this`.  This is the same walk as
// the private helper in the feature-34 batch (which is a different TU), so it
// is reimplemented here as an exported symbol.
void UncheckRadioButtonsInGroupImpl(S* s) {
    HWND hwndThis = s->m_hWnd;
    HWND hParent = ::GetParent(hwndThis);
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (pParent == nullptr) return;
    if (!::IsWindowEnabled(hwndThis)) return;

    bool tabStopChanged = false;
    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent, FALSE);
        if (hwndNext == nullptr || hwndNext == hwndThis) break;
        // Retail requires the sibling to be a CMFCButton (IsKindOf) with
        // m_bCheckStyle set; approximated by resolving the HWND and reading
        // m_bCheckStyle (0x130) directly.
        void* pObj = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndNext);
        if (pObj != nullptr &&
            *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x130) != 0) {
            LONG_PTR style = ::GetWindowLongPtrW(hwndNext, GWL_STYLE);
            if ((style & WS_TABSTOP) != 0) {
                ::SetWindowLongPtrW(hwndNext, GWL_STYLE, style & ~WS_TABSTOP);
                tabStopChanged = true;
            }
            if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) != 0) {
                *reinterpret_cast<int*>(reinterpret_cast<char*>(pObj) + 0x128) = 0;
                ::RedrawWindow(hwndNext, nullptr, nullptr,
                               RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
            }
        }
        hwndCurrent = hwndNext;
    }
    if (tabStopChanged) {
        LONG_PTR style = ::GetWindowLongPtrW(hwndThis, GWL_STYLE);
        ::SetWindowLongPtrW(hwndThis, GWL_STYLE, style | WS_TABSTOP);
    }
}

// CMFCButton::PreTranslateMessage retail (RVA 0x1e560): resolves the parent
// CWnd and posts a WM_COMMAND notification carrying the button's dialog ID.
inline void SendButtonCommandToParent(void* pThis) {
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_hWnd == nullptr) return;
    HWND hParent = ::GetParent(s->m_hWnd);
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (pParent == nullptr) return;
    HWND hwndParent = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pParent) + 0x40);
    WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
    ::SendMessageW(hwndParent, WM_COMMAND, MAKEWPARAM(wId, 0),
                   reinterpret_cast<LPARAM>(s->m_hWnd));
}

} // namespace

//=============================================================================
// 1. Message handlers
//=============================================================================

// OnSetCursor(CWnd*, UINT, UINT): when a custom cursor was installed, sets it
// while the pointer is over the button client area; otherwise defers to the
// base dispatch.  Transcribed from retail RVA 0x1e830.
// Symbol: ?OnSetCursor@CMFCButton@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCButton__IEAAHPEAVCWnd__II_Z(
    void* pThis, void* /*pWnd*/, unsigned int /*nHitTest*/, unsigned int /*message*/)
{
    if (pThis == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_hCursor != nullptr) {
        RECT rc;
        ::GetClientRect(s->m_hWnd, &rc);
        POINT pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(s->m_hWnd, &pt);
        if (::PtInRect(&rc, pt)) {
            ::SetCursor(s->m_hCursor);
            return 1;
        }
    }
    // retail tail-calls the CWnd base dispatch here
    return 0;
}

// OnSetFont(CFont*, BOOL): caches the CFont::m_hObject handle (offset 8) into
// the 0xb00 slot and redraws on request.  Retail RVA 0x1eb20.
// Symbol: ?OnSetFont@CMFCButton@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCButton__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int bRedraw)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->pMenuFont = (pFont != nullptr) ? *reinterpret_cast<void**>(reinterpret_cast<char*>(pFont) + 8)
                                      : nullptr;
    if (bRedraw != 0 && s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}

// OnSetImage(WPARAM, LPARAM): routes the bitmap (wParam==0) or icon
// (wParam==1) handle in lParam into the matching SetImageInternal overload.
// Retail RVA 0x1f410.
// Symbol: ?OnSetImage@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnSetImage_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long lParam)
{
    if (pThis == nullptr) return 0;
    if (wParam == 0) {
        impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
            pThis, reinterpret_cast<void*>(lParam), 0, 0, 1, 0, 0);
    } else if (wParam == 1) {
        impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
            pThis, reinterpret_cast<void*>(lParam), 0, 0, 0, 0, 0);
    }
    return 0;
}

// OnSize(UINT, int, int): copies the focus-draw flag pair, then defers to the
// base dispatch.  Retail RVA 0x1ec10.
// Symbol: ?OnSize@CMFCButton@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCButton__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    s->m_bDrawFocusB = s->m_bDrawFocusA;
    // retail tail-calls the base OnSize dispatch here
}

// OnTimer(UINT_PTR): while the autorepeat timer fires over a clicked/hovered
// radio-style button, repeats the BN_CLICKED-style WM_COMMAND to the parent.
// Retail RVA 0x1f0e0.
// Symbol: ?OnTimer@CMFCButton@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCButton__IEAAX_K_Z(
    void* pThis, unsigned long long nIDEvent)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (nIDEvent == 0xec0d && s->m_bHover != 0 && s->m_bClickStarted != 0) {
        SendButtonCommandToParent(pThis);
    }
    // retail tail-calls the CWnd base dispatch here
}

// OnUpdateToolTips(WPARAM, LPARAM): relays the mouse state to the tooltip
// helper.  The retail body (RVA 0x1f4f0) drives the tooltip-manager global
// (0x1803b25e8) which is not modeled; the guard checks are kept.
// Symbol: ?OnUpdateToolTips@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnUpdateToolTips_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr && (wParam & 0x40) != 0) {
            // TODO(clean-room): tooltip-manager update not modeled.
        }
    }
    return 0;
}

// PreCreateWindow(CREATESTRUCTW&): decodes the button style and forces the
// BS_OWNERDRAW bit (style = (style & ~BS_PUSHBUTTON) | BS_OWNERDRAW).
// Retail RVA 0x1cd30.
// Symbol: ?PreCreateWindow@CMFCButton@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCButton__MEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (pThis == nullptr || lpCreateStruct == nullptr) return FALSE;
    impl__InitStyle_CMFCButton__IEAAXK_Z(pThis, lpCreateStruct->style);
    lpCreateStruct->style = (lpCreateStruct->style & ~1u) | 0xA;
    // retail tail-calls the base PreCreateWindow, which returns TRUE
    return TRUE;
}

// PreSubclassWindow(): decodes the live window style, then forces
// BS_OWNERDRAW via ModifyStyle(1, 0xB, 0).  Retail RVA 0x1cd00.
// Symbol: ?PreSubclassWindow@CMFCButton@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCButton__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_hWnd == nullptr) return;
    LONG_PTR style = ::GetWindowLongPtrW(s->m_hWnd, GWL_STYLE);
    impl__InitStyle_CMFCButton__IEAAXK_Z(pThis, static_cast<unsigned long>(style));
    impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(s->m_hWnd, 1, 0xB, 0);
}

// PreTranslateMessage(MSG*): relays mouse messages to the tooltip helper,
// turns VK_RETURN into a WM_COMMAND click, and handles the radio-group
// keyboard navigation (VK_SPACE toggles, arrow keys move the check).
// Transcribed from retail RVA 0x1e560.
// Symbol: ?PreTranslateMessage@CMFCButton@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (pThis == nullptr || pMsg == nullptr) return 0;
    S* s = reinterpret_cast<S*>(pThis);

    // (a) Relay mouse messages to the tooltip helper.
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr &&
            pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) {
            ::SendMessageW(hwndHelper, 0x407 /* TTM_RELAYEVENT */, 0,
                           reinterpret_cast<LPARAM>(pMsg));
        }
    }

    if (pMsg->message == WM_KEYDOWN) {
        // (b) VK_RETURN behaves like a click.  Retail gates this on the global
        // animation pointer (0x1803be288) being null; that global is not
        // modeled, so the path is always taken.
        if (pMsg->wParam == VK_RETURN) {
            HWND hParent = ::GetParent(s->m_hWnd);
            void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
            if (pParent != nullptr) {
                SendButtonCommandToParent(pThis);
                return 1;
            }
        }

        // (c) Auto-toggle keyboard navigation.
        if (s->m_bAutoToggle != 0) {
            HWND hParent = ::GetParent(s->m_hWnd);
            void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
            if (pParent != nullptr) {
                switch (pMsg->wParam) {
                case VK_SPACE:
                    if (s->m_bRadioStyle != 0) {
                        s->m_bChecked = (s->m_bChecked == 0) ? 1 : 0;
                        if (s->m_hWnd != nullptr) {
                            ::RedrawWindow(s->m_hWnd, nullptr, nullptr,
                                           RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
                        }
                        SendButtonCommandToParent(pThis);
                        return 1;
                    }
                    break;
                case VK_LEFT:
                case VK_UP:
                    if (impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(pThis, 0) != 0) {
                        return 1;
                    }
                    break;
                case VK_RIGHT:
                case VK_DOWN:
                    if (impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(pThis, 1) != 0) {
                        return 1;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    // retail tail-calls the base PreTranslateMessage here
    return 0;
}

// SelectFont(CDC*): validates the cached menu font and selects it into the
// DC (or the stock DEFAULT_GUI_FONT when none is set).  The retail body
// (RVA 0x1eaa0) returns the previously selected CFont* through the CDC
// SelectObject virtual; the previous font is not tracked here.
// Symbol: ?SelectFont@CMFCButton@@MEAAPEAVCFont@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__SelectFont_CMFCButton__MEAAPEAVCFont__PEAVCDC___Z(
    void* pThis, void* pDC)
{
    if (pThis == nullptr) return nullptr;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->pMenuFont != nullptr &&
        ::GetObjectType(reinterpret_cast<HGDIOBJ>(s->pMenuFont)) != OBJ_FONT) {
        s->pMenuFont = nullptr;
    }
    HFONT hFont = reinterpret_cast<HFONT>(s->pMenuFont);
    HDC hdc = hdc_of(pDC);
    if (hdc != nullptr) {
        if (hFont == nullptr) {
            hFont = reinterpret_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
        }
        ::SelectObject(hdc, hFont);
    }
    // TODO(clean-room): previously selected CFont* not returned.
    return nullptr;
}

//=============================================================================
// 2. Image setter family
//=============================================================================

// SetAutorepeatMode(int): stores the autorepeat timer interval.  Retail RVA
// 0x1f0d0 (single store into m_nTimerID at 0x144).
// Symbol: ?SetAutorepeatMode@CMFCButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetAutorepeatMode_CMFCButton__QEAAXH_Z(
    void* pThis, int nMode)
{
    if (pThis == nullptr) return;
    reinterpret_cast<S*>(pThis)->m_nTimerID = nMode;
}

// SetImageInternal(UINT, UINT, int, UINT): resource-ID image setter.  Retail
// (RVA 0x1dd00) loads three icons from the module resources through an
// internal helper and forwards them to the HBITMAP overload; the resource
// loads are not modeled, so the primary handle slot is cleared instead.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXIIHI@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(
    void* pThis, unsigned int nID, unsigned int /*nImage*/,
    int /*bChecked*/, unsigned int /*nCheckedImage*/)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    if (nID == 0) return;
    // TODO(clean-room): icon-from-resource loads (internal 0x18001dc10) and
    // image-object setup not modeled.
    ImageHandleSlot(s) = nullptr;
}

// SetImageInternal(HBITMAP, int, HBITMAP, int, int, HBITMAP): bitmap image
// setter.  Retail (RVA 0x1da00) calls an InitStyle-like helper and drives the
// image objects; the primary bitmap is kept in the first image object's
// handle slot (0x1f8) which OnGetImage reads back.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXPEAUHBITMAP__@@H0HH0@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
    void* pThis, void* hBitmap, int /*bAutoDestroy*/, void* /*hBitmapHot*/,
    int /*bAutoDestroyHot*/, int /*bChecked*/, void* /*hBitmapChecked*/)
{
    if (pThis == nullptr) return;
    if (hBitmap != nullptr) {
        ImageHandleSlot(reinterpret_cast<S*>(pThis)) = hBitmap;
        // TODO(clean-room): hot/checked image objects not modeled.
    }
}

// SetImageInternal(HICON, int, HICON, int, HICON, int): icon image setter.
// Retail (RVA 0x1d6f0) resolves the icon dimensions via GetIconInfo/GetObject
// and drives the image objects; the observable subset kept here is the icon
// handle slot plus the m_sizeImage (0x148/0x14c) dimensions.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXPEAUHICON__@@H0H0H@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
    void* pThis, void* hIcon, int /*bAutoDestroy*/, void* /*hIconHot*/,
    int /*bChecked*/, void* /*hIconChecked*/, int /*bAutoDestroyHot*/)
{
    if (pThis == nullptr) return;
    if (hIcon != nullptr) {
        S* s = reinterpret_cast<S*>(pThis);
        ImageHandleSlot(s) = hIcon;
        ICONINFO ii;
        std::memset(&ii, 0, sizeof(ii));
        if (::GetIconInfo(reinterpret_cast<HICON>(hIcon), &ii)) {
            BITMAP bm;
            std::memset(&bm, 0, sizeof(bm));
            if (::GetObjectW(ii.hbmColor, sizeof(bm), &bm) != 0) {
                s->m_sizeImageCX = bm.bmWidth;
                s->m_sizeImageCY = bm.bmHeight;
            }
            if (ii.hbmColor != nullptr) ::DeleteObject(ii.hbmColor);
            if (ii.hbmMask != nullptr) ::DeleteObject(ii.hbmMask);
        }
        // TODO(clean-room): hot/checked image objects not modeled.
    }
}

// SetImage(UINT, UINT, UINT) / SetCheckedImage(UINT, UINT, UINT): route into
// the UINT SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d650 / 0x1d6d0.
// Symbol: ?SetImage@CMFCButton@@QEAAXIII@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXIII_Z(
    void* pThis, unsigned int nID, unsigned int nImage, unsigned int nCheckedImage)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(pThis, nID, nImage, 0, nCheckedImage);
}

// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXIII@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXIII_Z(
    void* pThis, unsigned int nID, unsigned int nImage, unsigned int nCheckedImage)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(pThis, nID, nImage, 1, nCheckedImage);
}

// SetImage(HBITMAP, int, HBITMAP, int, HBITMAP) / checked variant: route into
// the HBITMAP SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d620 / 0x1d6a0.
// Symbol: ?SetImage@CMFCButton@@QEAAXPEAUHBITMAP__@@H0H0@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXPEAUHBITMAP____H0H0_Z(
    void* pThis, void* hBitmap, int bAutoDestroy, void* hBitmapHot,
    int bAutoDestroyHot, void* hBitmapChecked)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
        pThis, hBitmap, bAutoDestroy, hBitmapHot, bAutoDestroyHot, 0, hBitmapChecked);
}

// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXPEAUHBITMAP__@@H0H0@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXPEAUHBITMAP____H0H0_Z(
    void* pThis, void* hBitmap, int bAutoDestroy, void* hBitmapHot,
    int bAutoDestroyHot, void* hBitmapChecked)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
        pThis, hBitmap, bAutoDestroy, hBitmapHot, bAutoDestroyHot, 1, hBitmapChecked);
}

// SetImage(HICON, int, HICON, int, HICON) / checked variant: route into the
// HICON SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d5f0 / 0x1d670.  Note the retail parameter order is
// (hIcon, bAutoDestroy, hIconHot, hIconChecked, bAutoDestroyHot).
// Symbol: ?SetImage@CMFCButton@@QEAAXPEAUHICON__@@H00H@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXPEAUHICON____H00H_Z(
    void* pThis, void* hIcon, int bAutoDestroy, void* hIconHot,
    void* hIconChecked, int bAutoDestroyHot)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
        pThis, hIcon, bAutoDestroy, hIconHot, 0, hIconChecked, bAutoDestroyHot);
}

// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXPEAUHICON__@@H00H@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXPEAUHICON____H00H_Z(
    void* pThis, void* hIcon, int bAutoDestroy, void* hIconHot,
    void* hIconChecked, int bAutoDestroyHot)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
        pThis, hIcon, bAutoDestroy, hIconHot, 1, hIconChecked, bAutoDestroyHot);
}

//=============================================================================
// 3. Cursor / std image
//=============================================================================

// SetMouseCursor(HICON): installs a custom cursor.  Retail RVA 0x1e7d0
// (single store into m_hCursor at 0xb08).
// Symbol: ?SetMouseCursor@CMFCButton@@QEAAXPEAUHICON__@@@Z
extern "C" void MS_ABI impl__SetMouseCursor_CMFCButton__QEAAXPEAUHICON_____Z(
    void* pThis, void* hCursor)
{
    if (pThis == nullptr) return;
    reinterpret_cast<S*>(pThis)->m_hCursor = reinterpret_cast<HICON>(hCursor);
}

// SetMouseCursorHand(): installs the standard hand cursor.  Retail
// (RVA 0x1e7e0) takes it from a cached visual-manager cursor; LoadCursorW
// with IDC_HAND is the public equivalent.
// Symbol: ?SetMouseCursorHand@CMFCButton@@QEAAXXZ
extern "C" void MS_ABI impl__SetMouseCursorHand_CMFCButton__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): uses LoadCursorW(IDC_HAND) instead of the cached
    // visual-manager cursor (0x18006c790).
    reinterpret_cast<S*>(pThis)->m_hCursor =
        ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_HAND));
}

// SetStdImage(CMenuImages::IMAGES_IDS, IMAGE_STATE, IMAGE_STATE): stores the
// standard-image id/state triplet.  Retail (RVA 0x1dd80) also fills the
// 8-byte m_sizeImage slot from CMenuImages::GetImage (internal 0x18008f600)
// and a vtable-side-effect call; the image lookup is not modeled, so the size
// slot is cleared.
// Symbol: ?SetStdImage@CMFCButton@@QEAAXW4IMAGES_IDS@CMenuImages@@W4IMAGE_STATE@3@0@Z
extern "C" void MS_ABI impl__SetStdImage_CMFCButton__QEAAXW4IMAGES_IDS_CMenuImages__W4IMAGE_STATE_3_0_Z(
    void* pThis, int nImageId, int nState, int nStateHot)
{
    if (pThis == nullptr) return;
    S* s = reinterpret_cast<S*>(pThis);
    // TODO(clean-room): CMenuImages::GetImage lookup not modeled.
    s->m_sizeImageCX = 0;
    s->m_sizeImageCY = 0;
    s->m_nImageIndex = nImageId;          // 0xae8
    s->m_nCheckedImageIndex = nStateHot;  // 0xaec
    s->m_nState = nState;                 // 0xaf0
}

//=============================================================================
// 4. Sizing / radio group / accessibility
//=============================================================================

// SizeToContent(BOOL): computes the size the button needs for its text and
// image plus the fixed margins (GetImageHorzMargin = 10, GetVertMargin = 5),
// and resizes the window when bCalcOnly is FALSE.  The retail body
// (RVA 0x1e280) measures the text through internal font/CString helpers; a
// DrawTextW DT_CALCRECT pass is the equivalent.  CSize is returned through
// the hidden return slot (this in %rcx, slot in %rdx).
// Symbol: ?SizeToContent@CMFCButton@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_hWnd == nullptr) return pRet;

    // Text extent using the cached menu font (or the stock default GUI font).
    int textW = 0;
    int textH = 0;
    wchar_t buf[1024];
    const int nLen = ::GetWindowTextW(s->m_hWnd, buf, 1024);
    HDC hdc = ::GetDC(s->m_hWnd);
    if (hdc != nullptr) {
        HFONT hFont = reinterpret_cast<HFONT>(s->pMenuFont);
        if (hFont == nullptr) {
            hFont = reinterpret_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
        }
        HGDIOBJ hOld = ::SelectObject(hdc, hFont);
        RECT rc = {0, 0, 0, 0};
        if (nLen > 0) {
            ::DrawTextW(hdc, buf, nLen, &rc, DT_CALCRECT | DT_SINGLELINE);
            textW = rc.right - rc.left;
            textH = rc.bottom - rc.top;
        }
        ::SelectObject(hdc, hOld);
        ::ReleaseDC(s->m_hWnd, hdc);
    }

    const int iconW = s->m_sizeImageCX;
    const int iconH = s->m_sizeImageCY;
    const int marginX = 10; // GetImageHorzMargin
    const int marginY = 5;  // GetVertMargin
    if (s->m_bImageIsTiled != 0) {
        // Tiled-image layout: icon sits beside the text.
        pRet->cx = iconW + marginX + textW + (textW > 0 ? marginX : 0);
        pRet->cy = 2 * marginY + (textH > iconH ? textH : iconH);
    } else {
        pRet->cx = marginX + (textW > iconW ? textW : iconW) + (textW > 0 ? marginX : 0);
        pRet->cy = iconH + marginY + textH + (iconH > 0 ? marginY : 0) +
                   (textH > 0 ? marginY : 0);
    }

    if (bCalcOnly == 0) {
        ::SetWindowPos(s->m_hWnd, nullptr, 0, 0, pRet->cx, pRet->cy,
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return pRet;
}

// UncheckRadioButtonsInGroup(): unchecks every checked CMFCButton radio
// sibling in the WS_GROUP, removing WS_TABSTOP from them, then restores
// WS_TABSTOP on `this`.  Transcribed from retail RVA 0x1efb0 (same walk as
// the private helper in the feature-34 batch, reimplemented here).
// Symbol: ?UncheckRadioButtonsInGroup@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__UncheckRadioButtonsInGroup_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    UncheckRadioButtonsInGroupImpl(reinterpret_cast<S*>(pThis));
}

// get_accState(tagVARIANT, VARIANT*): MSAA state for the child variant.
// Retail (RVA 0x1fb00) resolves the child index through an internal
// VariantChangeType helper that fills *pvarVal, then ORs the button state
// bits into pvarVal->lVal: STATE_SYSTEM_FOCUSED (0x80) while a click is in
// progress, and for a checked button STATE_SYSTEM_SELECTED (0x2) when
// m_bCheckStyle is set, else STATE_SYSTEM_CHECKED (0x10).
// Symbol: ?get_accState@CMFCButton@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCButton__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, const VARIANT& varChild, VARIANT* pvarVal)
{
    if (pvarVal == nullptr) return E_INVALIDARG;
    if (varChild.vt != VT_I4) {
        // TODO(clean-room): retail converts the child variant through an
        // internal VariantChangeType helper (0x180290620); only VT_I4 is
        // modeled.
        return E_INVALIDARG;
    }
    pvarVal->vt = VT_I4;
    pvarVal->lVal = varChild.lVal;
    if (pThis == nullptr) return S_OK;
    S* s = reinterpret_cast<S*>(pThis);
    if (s->m_bClickStarted != 0) pvarVal->lVal |= 0x80;
    if (s->m_bChecked != 0) {
        if (s->m_bCheckStyle != 0) pvarVal->lVal |= 0x2;   // STATE_SYSTEM_SELECTED
        else pvarVal->lVal |= 0x10;                        // STATE_SYSTEM_CHECKED
    }
    return S_OK;
}
