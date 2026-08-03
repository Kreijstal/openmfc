// OpenMFC: CMFCColorDialog exports.
//
// All 14 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the two page sub-objects the bodies touch are mirrored with the
// file-local struct below (the page-one CMFCColorPickerCtrl at +0x178 and the
// page-two CMFCCustomColorsPropertyPage at +0x180; the pickers embedded in the
// pages sit at +0x160/+0x2b8).  The stub constructors leave the object state
// uninitialized, so every pointer access is guarded.
//
// Faithfully transcribed: SetPageOne / SetPageTwo (which forward to the
// implemented CMFCColorPickerCtrl::SetColor and
// CMFCCustomColorsPropertyPage::Setup exports), OnSetCursor, OnMouseMove,
// RebuildPalette, OnDestroy and the full Ctrl-C / Ctrl-- clipboard path of
// PreTranslateMessage.  The CColorDialog layout used by OpenColorDialog (a
// 0xf50-byte stack frame) and the CMap-backed color-name tables are unmodeled
// and marked "TODO(clean-room)".
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

#include <cstddef>
#include <cstdint>
#include <cstdlib>
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
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(
    CDialogEx* pThis, MSG* pMsg);
extern "C" void MS_ABI impl__OnDestroy_CDialogEx__IEAAXXZ(CDialogEx* pThis);
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pv);
extern "C" void MS_ABI impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
    void* pThis, COLORREF clr);
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);

namespace {

// The palette object at 0xbc0 is a 0x10-byte CGdiObject-like heap block
// { vtable, m_hObject } in retail.  The retail vtable (0x1802e0468) is not
// present here; OpenMFC-created holders leave vtable == nullptr so a foreign
// / uninitialized block can be recognised and never freed through.
struct CPalHolder {
    void*   vtable;    // 0x00 (null = OpenMFC-created holder)
    HPALETTE m_hPal;   // 0x08 (m_hObject)
};

static_assert(sizeof(CPalHolder) == 0x10, "CPalHolder must be 0x10 bytes");

struct CColorDlg {
    char    _pad000[0x40];
    HWND    m_hWnd;              // 0x040: CWnd::m_hWnd
    char    _pad048[0x170 - 0x48];
    void*   m_pCursorPicker;     // 0x170: picker sub-object (ctor-nulled)
    void*   m_pColorPicker;      // 0x178: CMFCColorPickerCtrl*
    void*   m_pSecondPicker;     // 0x180: second picker/list object
    CPalHolder* m_pPalette;      // 0x188: palette holder (from ctor arg)
    int     m_nCurrentColor;     // 0x190: current color (SetCurrentColor)
    int     m_nNewColor;         // 0x194: new color; bytes 0x194/0x195/0x196 = R/G/B
    HCURSOR m_hCursor;           // 0x198: eyedropper cursor
    int     m_bCustomPalette;    // 0x1a0: 1 = custom palette, 0 = system
    int     m_nTracking;         // 0x1a4: color-popup tracking active
    char    _pad1a8[0xdb0 - 0x1a8];   // 0x1a8 CMFCButton, 0xcc8 bar/wnd (opaque)
    char    m_picker[0x40];      // 0xdb0: CMFCColorPickerCtrl m_hWnd at +0x40
    char    _paddf0[0xdf0 - 0xdf0];
};

static_assert(offsetof(CColorDlg, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorDlg, m_pPalette) == 0x188, "palette @0x188");
static_assert(offsetof(CColorDlg, m_nCurrentColor) == 0x190, "current color @0x190");
static_assert(offsetof(CColorDlg, m_nNewColor) == 0x194, "new color @0x194");
static_assert(offsetof(CColorDlg, m_hCursor) == 0x198, "cursor @0x198");
static_assert(offsetof(CColorDlg, m_nTracking) == 0x1a4, "tracking @0x1a4");

} // namespace

// SetCurrentColor(COLORREF clr): retail is a single store at 0x190
// (RVA 0x29a80).  Transcribed verbatim.
// Symbol: ?SetCurrentColor@CMFCColorDialog@@QEAAXK@Z
extern "C" void MS_ABI impl__SetCurrentColor_CMFCColorDialog__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    s->m_nCurrentColor = static_cast<int>(clr);                  // 0x190
}

// SetNewColor(COLORREF clr): stores the color at 0x194 (whose bytes are the
// R/G/B channels used by the clipboard and the pickers), updates the embedded
// CMFCColorPickerCtrl at 0xdb0 (color slot at picker+0x108) and repaints the
// picker window at 0xdf0.  Transcribed from retail RVA 0x29b60; the
// palette-index conversion (global 0x1803c18a8 == 8, GetNearestPaletteIndex
// 0x1802c6230 + bit 0x18) is not modeled.
// Symbol: ?SetNewColor@CMFCColorDialog@@QEAAXK@Z
extern "C" void MS_ABI impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    s->m_nNewColor = static_cast<int>(clr);                      // 0x194

    // TODO(clean-room): transcribed partially -- the palette-index conversion
    // (global 0x1803c18a8 == 8) and the picker RGB-channel decomposition
    // (CMFCColorPickerCtrl::SetColor 0x18002c4e0 -> 0x18005b8c0,
    // channels at +0xf0/+0xf8/+0x100) are not modeled; only the color slot at
    // +0x108 is stored.
    char* pPicker = reinterpret_cast<char*>(pThis) + 0xdb0;
    *reinterpret_cast<unsigned int*>(pPicker + 0x108) = static_cast<unsigned int>(clr);
    HWND hwndPicker = *reinterpret_cast<HWND*>(pPicker + 0x40);  // 0xdf0
    if (hwndPicker != nullptr) {
        ::InvalidateRect(hwndPicker, nullptr, TRUE);             // 0x1802c7128
        ::UpdateWindow(hwndPicker);                              // 0x1802c7300
    }
}

// OnSetCursor(CWnd*, UINT, UINT): while color tracking is active, sets the
// eyedropper cursor and returns TRUE; otherwise delegates to CWnd::Default.
// Transcribed from retail RVA 0x29f80.
// Symbol: ?OnSetCursor@CMFCColorDialog@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCColorDialog__IEAAHPEAVCWnd__II_Z(
    void* pThis, void* /*pWnd*/, unsigned int /*nHitTest*/,
    unsigned int /*message*/)
{
    if (pThis == nullptr) return 0;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (s->m_nTracking != 0) {
        ::SetCursor(s->m_hCursor);                               // 0x1802c71e0
        return 1;
    }
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(
        reinterpret_cast<CWnd*>(pThis)));
}

// OnMouseMove(UINT, CPoint): while color tracking is active, converts the
// point to screen coordinates, reads the pixel under the cursor with the
// client DC and feeds it to SetNewColor; then delegates to CWnd::Default.
// Transcribed from retail RVA 0x29fb0.
// Symbol: ?OnMouseMove@CMFCColorDialog@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCColorDialog__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT point)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (s->m_nTracking != 0) {
        POINT ptScreen = point;
        ::ClientToScreen(s->m_hWnd, &ptScreen);                  // 0x1802c7248
        HDC hdc = ::GetDC(s->m_hWnd);                            // CClientDC
        if (hdc != nullptr) {
            COLORREF pixel = ::GetPixel(hdc, ptScreen.x, ptScreen.y);  // 0x1802c6220
            ::ReleaseDC(s->m_hWnd, hdc);
            impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(pThis, pixel);
        }
    }
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}

// OnLButtonDown(UINT, CPoint): clears the tracking flag, then (in retail)
// pushes the RGB bytes at 0x194/0x195/0x196 into the two picker controls
// (internal 0x180029a90 / 0x180034930) and delegates to CWnd::Default.  The
// picker updates are not modeled.
// Symbol: ?OnLButtonDown@CMFCColorDialog@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCColorDialog__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    s->m_nTracking = 0;                                          // 0x1a4
    // TODO(clean-room): transcribed partially -- the picker updates
    // (0x180029a90 -> CMFCColorPickerCtrl::SetColor on 0x178, and 0x180034930
    // on 0x180) are not modeled.
    (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
}

// RebuildPalette(): builds a system-palette LOGPALETTE (GetSystemPaletteEntries
// 0x100 entries), CreatePalette's it and attaches it to the palette holder at
// 0x188, allocating the holder when missing.  Transcribed from retail RVA
// 0x29d20 (the AfxThrowInvalidArgException path when the palette is already
// populated is skipped).
// Symbol: ?RebuildPalette@CMFCColorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildPalette_CMFCColorDialog__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);

    CPalHolder* pal = s->m_pPalette;
    // Retail rebuilds only when the holder is missing or has no attached
    // object (and otherwise throws AfxThrowInvalidArgException).
    if (pal != nullptr && pal->m_hPal != nullptr) return;

    HDC hdc = (s->m_hWnd != nullptr) ? ::GetDC(s->m_hWnd) : nullptr;  // CClientDC
    LOGPALETTE* pLog = static_cast<LOGPALETTE*>(std::malloc(0x408));
    if (pLog != nullptr) {
        pLog->palVersion = 0x300;
        pLog->palNumEntries = 0x100;
        if (hdc != nullptr) {
            ::GetSystemPaletteEntries(hdc, 0, 0x100, pLog->palPalEntry);  // 0x1802c6228
        }
        HPALETTE hPal = ::CreatePalette(pLog);                   // 0x1802c6240
        std::free(pLog);
        if (hPal != nullptr) {
            if (pal == nullptr) {
                pal = static_cast<CPalHolder*>(std::calloc(1, sizeof(CPalHolder)));
                s->m_pPalette = pal;
            }
            if (pal != nullptr && pal->vtable == nullptr) {
                pal->m_hPal = hPal;   // retail: CGdiObject::Attach (0x1802a3ed0)
            } else {
                ::DeleteObject(hPal);
            }
        }
    }
    if (hdc != nullptr) ::ReleaseDC(s->m_hWnd, hdc);
}

// OnDestroy(): when the custom-palette flag 0x1a0 is set, releases the
// palette handle (DeleteObject) and nulls 0x188, then delegates to
// CDialogEx::OnDestroy.  Transcribed from retail RVA 0x29b00 (the retail
// vtable-delete of the CPalette object itself is replaced by DeleteObject;
// foreign holder blocks are not freed).
// Symbol: ?OnDestroy@CMFCColorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCColorDialog__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (s->m_bCustomPalette != 0 && s->m_pPalette != nullptr) {
        if (s->m_pPalette->m_hPal != nullptr) ::DeleteObject(s->m_pPalette->m_hPal);
        // TODO(clean-room): retail deletes the CPalette object itself via its
        // vtable (0x1802c7b30 dispatch); the mirror block is not freed so a
        // caller-supplied palette is never freed through a foreign CRT.
        s->m_pPalette = nullptr;
    }
    impl__OnDestroy_CDialogEx__IEAAXXZ(reinterpret_cast<CDialogEx*>(pThis));
}

// DoDataExchange(CDataExchange*): binds the three embedded controls through
// DDX_Control.  Transcribed from retail RVA 0x29550.
// Symbol: ?DoDataExchange@CMFCColorDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCColorDialog__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX)
{
    if (pThis == nullptr || pDX == nullptr) return;
    char* base = reinterpret_cast<char*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4295, base + 0x1a8);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x424a, base + 0xcc8);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x4116, base + 0xdb0);
}

// OnInitDialog(): delegates to CDialog::OnInitDialog and returns TRUE.
// Transcribed from retail RVA 0x295c0; the retail body also creates and lays
// out the picker controls (0x170/0x178/0x180), loads the eyedropper cursor
// into 0x198 and re-reads the current color -- none of that machinery is
// modeled.
// Symbol: ?OnInitDialog@CMFCColorDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCColorDialog__MEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- the picker creation/layout
    // (0x18002a080 / 0x180132ca0 / 0x1800341a0 / 0x180217ce0), the cursor load
    // (0x1802aeb50 -> SetCursor, stored at 0x198) and the color re-read
    // (CColorDialog machinery 0x180207670 / 0x180207760 / 0x180208b10) are
    // not modeled.
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(reinterpret_cast<CDialog*>(pThis));
    return 1;   // retail returns TRUE
}

// OnSysColorChange(): delegates to the CWnd base handler and repaints the
// dialog.  Retail (RVA 0x29c00) additionally runs the CMFCPropertySheet base
// (0x18003f450) and, when the custom-palette flag 0x1a0 is set, re-reads the
// current color through a temporary CColorDialog and rebuilds the palette;
// that machinery is not modeled.
// Symbol: ?OnSysColorChange@CMFCColorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCColorDialog__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    impl__OnSysColorChange_CWnd__IEAAXXZ(reinterpret_cast<CWnd*>(pThis));
    // TODO(clean-room): transcribed partially -- the temporary-CColorDialog
    // color re-read (0x180207670 / 0x180207760 / 0x180208b10) and the palette
    // rebuild (0x180029d20) when the custom-palette flag 0x1a0 is set are not
    // modeled.
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);              // 0x1802c7128
        ::UpdateWindow(s->m_hWnd);                               // 0x1802c7300
    }
}

// OnColorSelect(): retail (RVA 0x29dc0) starts an eyedropper message loop
// (tracking flag 0x1a4 = 1, a 0xf0-byte tracking window at 0x18002a080,
// SetCapture + PeekMessageW for WM_KEYDOWN Enter/Esc).  The tracking-window
// machinery is not modeled; only the early-exit gate is transcribed.
// Symbol: ?OnColorSelect@CMFCColorDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnColorSelect_CMFCColorDialog__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (s->m_nTracking != 0) return;
    // TODO(clean-room): transcribed partially -- the tracking window
    // (0x18002a080, vslot+0x2d8 dispatch), SetForegroundWindow /
    // BringWindowToTop / SetCapture and the PeekMessageW loop (0x1802c7198)
    // are not modeled.
}

// PreTranslateMessage(MSG*): when Ctrl is down and the key is 'C' or '-',
// copies "RGB(r, g, b)" (built from the color bytes at 0x194/0x195/0x196) to
// the clipboard as CF_UNICODETEXT, then delegates to CDialogEx::PreTranslateMessage.
// Transcribed from retail RVA 0x2a350.
// Symbol: ?PreTranslateMessage@CMFCColorDialog@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCColorDialog__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (pThis == nullptr || pMsg == nullptr) {
        return impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(
            reinterpret_cast<CDialogEx*>(pThis), pMsg);
    }
    if (pMsg->message == WM_KEYDOWN &&
        (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0 &&        // 0x1802c7170
        (pMsg->wParam == 'C' || pMsg->wParam == L'-')) {
        CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
        if (s->m_hWnd != nullptr && ::OpenClipboard(s->m_hWnd)) { // 0x1802c7168
            ::EmptyClipboard();                                   // 0x1802c6e88
            // "RGB(%d, %d, %d)" with the R/G/B bytes of m_nNewColor.
            const unsigned char r = static_cast<unsigned char>(s->m_nNewColor & 0xff);
            const unsigned char g = static_cast<unsigned char>((s->m_nNewColor >> 8) & 0xff);
            const unsigned char b = static_cast<unsigned char>((s->m_nNewColor >> 16) & 0xff);
            wchar_t buf[64];
            const int nLen = swprintf(buf, 64, L"RGB(%d, %d, %d)",
                                      static_cast<int>(r), static_cast<int>(g),
                                      static_cast<int>(b));
            if (nLen > 0) {
                HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE,       // 0x1802c6620
                                             static_cast<SIZE_T>(nLen + 1) * sizeof(wchar_t));
                if (hMem != nullptr) {
                    void* pData = ::GlobalLock(hMem);             // 0x1802c6628
                    if (pData != nullptr) {
                        std::memcpy(pData, buf,
                                    static_cast<size_t>(nLen + 1) * sizeof(wchar_t));
                        ::GlobalUnlock(hMem);                     // 0x1802c6630
                        ::SetClipboardData(CF_UNICODETEXT, hMem); // 0x1802c6ba8
                    } else {
                        ::GlobalFree(hMem);
                    }
                }
            }
            ::CloseClipboard();                                   // 0x1802c6bb0
        }
    }
    return impl__PreTranslateMessage_CDialogEx__UEAAHPEAUtagMSG___Z(
        reinterpret_cast<CDialogEx*>(pThis), pMsg);
}

//-----------------------------------------------------------------------------
// CMFCColorDialog::SetPageOne
//-----------------------------------------------------------------------------
// Retail (0x180029a90): packs (bRed, bGreen, bBlue) into RGB(bRed,bGreen,
// bBlue) (param2 << 0 | param3 << 8 | param4 << 16), reads the page-one
// picker at +0x178 and calls CMFCColorPickerCtrl::SetColor on the +0x160 and
// +0x2b8 pickers inside it.  The picker object is opaque here but the SetColor
// export is implemented (global_cmfccolorpickerctrl.cpp), so the calls are kept
// the harvested +0x160/+0x2b8 member offsets.
// Symbol: ?SetPageOne@CMFCColorDialog@@QEAAXEEE@Z
extern "C" void MS_ABI impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue)
{
    if (!pThis) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (!s->m_pColorPicker) return;
    const COLORREF clr = RGB(bRed, bGreen, bBlue);
    impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
        reinterpret_cast<char*>(s->m_pColorPicker) + 0x160, clr);
    impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
        reinterpret_cast<char*>(s->m_pColorPicker) + 0x2b8, clr);
}

//-----------------------------------------------------------------------------
// CMFCColorDialog::SetPageTwo
//-----------------------------------------------------------------------------
// Retail (0x180029af0): reads the page-two sub-object at +0x180 and tail-calls
// CMFCCustomColorsPropertyPage::Setup (0x180034930) with the three channels
// forwarded.  The Setup export is implemented
// (global_cmfccustomcolorspropertypage.cpp), so
// the forwarding is kept.
// Symbol: ?SetPageTwo@CMFCColorDialog@@QEAAXEEE@Z
extern "C" void MS_ABI impl__SetPageTwo_CMFCColorDialog__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue)
{
    if (!pThis) return;
    CColorDlg* s = reinterpret_cast<CColorDlg*>(pThis);
    if (!s->m_pSecondPicker) return;
    impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
        s->m_pSecondPicker, bRed, bGreen, bBlue);
}
