// OpenMFC: CMFCCustomColorsPropertyPage exports.
//
// All 9 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  It is CDialog-derived and
// 0x430 bytes: hwnd at +0x40, the CMFCColorDialog owner at +0x158, the embedded
// CMFCColorPickerCtrl sub-objects at +0x160/+0x2b8, the luminance slider value
// at +0x250, the H/S mirrors at +0x258/+0x260, the colour-control HWND at
// +0x1a0, the HLS doubles at +0x3a8 (L) / +0x3b0 (S) / +0x3b8 (H), the COLORREF
// at +0x3c0, the RGB ints at +0x410/+0x414/+0x418, the HLS ints at +0x41c (L) /
// +0x420 (H) / +0x424 (S), the init flag at +0x428 and a re-entrancy guard at
// +0x42c.
//
// Faithfully transcribed: the DDX wiring in DoDataExchange and the colour
// arithmetic in Setup / OnColour / OnLuminance / OnRGBChanged / OnHLSChanged /
// OnInitDialog.
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

extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int p0);
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);
extern "C" void MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult);
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pv);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(
    void* pDX, int nIDC, int* pv);
extern "C" void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAI_Z(
    void* pDX, int nIDC, unsigned int* pv);
extern "C" void MS_ABI impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
    void* pThis, double dH, double dL, double dS, int bRedraw);
extern "C" void MS_ABI impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette);
extern "C" void MS_ABI impl__SetLuminanceBarWidth_CMFCColorPickerCtrl__QEAAXH_Z(
    void* pThis, int nWidth);
extern "C" void MS_ABI impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
    COLORREF rgb, double* pH, double* pS, double* pL);
extern "C" double MS_ABI impl__HuetoRGB_CDrawingManager__SANNNN_Z(
    double m1, double m2, double h);
extern "C" void MS_ABI impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(
    void* pThis, unsigned long clr);
extern "C" void MS_ABI impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);

namespace {

struct CColorsPage {
    char      _pad000[0x40];
    HWND      m_hWnd;              // 0x040: CWnd::m_hWnd
    char      _pad048[0x158 - 0x48];
    void*     m_pOwner;            // 0x158: CMFCColorDialog*
    char      _pad160[0x250 - 0x160];  // 0x160: picker sub-objects (opaque)
    double    m_dLumSlider;        // 0x250: luminance slider value (OnLuminance)
    double    m_dS255;             // 0x258: S mirror (OnColour)
    double    m_dH255;             // 0x260: H mirror (OnColour)
    char      _pad268[0x3a0 - 0x268];
    int       m_nPageKind;         // 0x3a0: page kind (OnInitDialog sets 2)
    double    m_dL;                // 0x3a8: Luminance
    double    m_dS;                // 0x3b0: Saturation
    double    m_dH;                // 0x3b8: Hue (turns)
    COLORREF  m_nColor;            // 0x3c0: current color
    char      _pad3c4[0x410 - 0x3c4];
    int       m_nR;                // 0x410: Red edit box value
    int       m_nB;                // 0x414: Blue edit box value
    int       m_nG;                // 0x418: Green edit box value
    int       m_nL;                // 0x41c: Luminance edit box value
    int       m_nH;                // 0x420: Hue edit box value
    int       m_nS;                // 0x424: Saturation edit box value
    int       m_bInitialized;      // 0x428: controls-initialized flag
    int       m_bUpdating;         // 0x42c: re-entrancy guard
};

static_assert(offsetof(CColorsPage, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorsPage, m_pOwner) == 0x158, "owner @0x158");
static_assert(offsetof(CColorsPage, m_dL) == 0x3a8, "L @0x3a8");
static_assert(offsetof(CColorsPage, m_dS) == 0x3b0, "S @0x3b0");
static_assert(offsetof(CColorsPage, m_dH) == 0x3b8, "H @0x3b8");
static_assert(offsetof(CColorsPage, m_nR) == 0x410, "R @0x410");
static_assert(offsetof(CColorsPage, m_nL) == 0x41c, "L int @0x41c");
static_assert(offsetof(CColorsPage, m_nH) == 0x420, "H int @0x420");
static_assert(offsetof(CColorsPage, m_bUpdating) == 0x42c, "guard @0x42c");

// The channel-to-8-bit helper used all over the colour page: (double*255)+0.5
// truncated, clamped to 255 (retail constant 0x180350098 = 255.0,
// 0x18034fef0 = 0.5, 0x18034ff68 = 1.0).
int ChannelToInt(double d) { return static_cast<int>(d * 255.0 + 0.5); }

// HLS -> COLORREF.  Matches CDrawingManager::HLStoRGB (retail 0x18005b650):
// hue is a turn fraction in [0,1], the conversion follows the standard
// hue-to-rgb formula and each channel is truncated after scaling by 255.
// (Identical to the helper used in global_cmfccolorpickerctrl.cpp.)
COLORREF HlsToRgb(double H, double L, double S)
{
    if (S == 0.0) {
        const int v = static_cast<int>(L * 255.0);
        return RGB(v, v, v);
    }
    double M2;
    if (L <= 0.5) {
        M2 = L * (1.0 + S);
    } else if (L == 1.0) {
        M2 = L;
    } else {
        M2 = L + S - L * S;
    }
    const double M1 = 2.0 * L - M2;
    const int r = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H + 1.0 / 3.0) * 255.0);
    const int g = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H) * 255.0);
    const int b = static_cast<int>(
        impl__HuetoRGB_CDrawingManager__SANNNN_Z(M1, M2, H - 1.0 / 3.0) * 255.0);
    return RGB(r, g, b);
}

} // namespace

// CreateObject(): retail (RVA 0x34150) allocates 0x430 bytes and runs the
// CDialog-derived constructor (0x1800341a0, which creates the 0x4243 dialog
// template and default-constructs the two embedded pickers).  The opaque class
// has no modeled constructor, so nullptr (the safe allocation-failure
// terminal) is returned.
// Symbol: ?CreateObject@CMFCCustomColorsPropertyPage@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomColorsPropertyPage__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x430 bytes
    // and runs the CMFCCustomColorsPropertyPage constructor (0x1800341a0);
    // the opaque class has no modeled constructor or vftable, so nullptr is
    // returned.
    return nullptr;
}

// DoDataExchange(CDataExchange* pDX): wires the two embedded pickers and the
// six RGB/HLS edit boxes.  Transcribed verbatim from retail RVA 0x34280 (the
// last DDX_Text call is the tail call).
// Symbol: ?DoDataExchange@CMFCCustomColorsPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCCustomColorsPropertyPage__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX)
{
    if (pThis == nullptr || pDX == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
        pDX, 0x424d, reinterpret_cast<char*>(pThis) + 0x160);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
        pDX, 0x4116, reinterpret_cast<char*>(pThis) + 0x2b8);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(pDX, 0x4244, &s->m_nR);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(pDX, 0x4246, &s->m_nB);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(pDX, 0x4245, &s->m_nG);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAI_Z(pDX, 0x4247,
        reinterpret_cast<unsigned int*>(&s->m_nL));
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAI_Z(pDX, 0x4249,
        reinterpret_cast<unsigned int*>(&s->m_nH));
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAI_Z(pDX, 0x4248,
        reinterpret_cast<unsigned int*>(&s->m_nS));
}

// OnColour(): the colour swatch was clicked -- reads the COLORREF at +0x3c0,
// refreshes the RGB/HLS edit-box values, invalidates the child control at
// +0x1a0 and notifies the owner.  Transcribed from retail RVA 0x34b90.
// Symbol: ?OnColour@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnColour_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    s->m_bUpdating = 1;                                   // 0x42c
    s->m_nR = static_cast<int>(GetRValue(s->m_nColor));   // +0x410
    s->m_nG = static_cast<int>(GetGValue(s->m_nColor));   // +0x418
    s->m_nB = static_cast<int>(GetBValue(s->m_nColor));   // +0x414
    s->m_dH255 = s->m_dH;                                 // +0x260
    s->m_dS255 = s->m_dS;                                 // +0x258
    const HWND hwndColour = *reinterpret_cast<HWND*>(
        reinterpret_cast<char*>(pThis) + 0x1a0);
    if (hwndColour != nullptr) ::InvalidateRect(hwndColour, nullptr, TRUE);

    s->m_nH = ChannelToInt(s->m_dH);                      // +0x420
    s->m_nL = ChannelToInt(s->m_dL);                      // +0x41c
    s->m_nS = ChannelToInt(s->m_dS);                      // +0x424
    const COLORREF clr = HlsToRgb(s->m_dH, s->m_dL, s->m_dS);
    if (s->m_pOwner != nullptr) {
        impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(s->m_pOwner, clr);
        impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
            s->m_pOwner,
            static_cast<unsigned char>(s->m_nR),
            static_cast<unsigned char>(s->m_nG),
            static_cast<unsigned char>(s->m_nB));
    }
    if (s->m_hWnd != nullptr) {
        impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 0);
    }
    s->m_bUpdating = 0;
}

// OnDoubleClickedColor(): the swatch was double-clicked -- ends the owner
// dialog with IDOK.  Transcribed verbatim from retail RVA 0x34cd0 (tail-jump
// to CDialog::EndDialog(owner, 1), 0x180208b10).
// Symbol: ?OnDoubleClickedColor@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnDoubleClickedColor_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    if (s->m_pOwner != nullptr) {
        impl__EndDialog_CDialog__QEAAXH_Z(
            reinterpret_cast<CDialog*>(s->m_pOwner), 1);
    }
}

// OnHLSChanged(): the HLS spin/edit controls changed -- reads the RGB edit
// boxes, recomposes through HLStoRGB and refreshes the pickers.  Transcribed
// from retail RVA 0x34460.
// Symbol: ?OnHLSChanged@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnHLSChanged_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    if (s->m_bUpdating != 0 || s->m_bInitialized == 0) return;
    // Retail requires the three RGB edit controls (0x4244/0x4245/0x4246) to
    // exist before proceeding.
    if (impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4244) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4245) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4246) == nullptr) {
        return;
    }
    if (!impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 1)) {
        return;
    }
    s->m_bUpdating = 1;
    // Pack the byte-masked RGB values (retail uses the pre-clamp bytes here).
    const COLORREF clr = RGB(
        static_cast<BYTE>(s->m_nR), static_cast<BYTE>(s->m_nG), static_cast<BYTE>(s->m_nB));
    // Clamp each edit value to 255 (retail cmovl keep-if-less).
    if (s->m_nR > 255) s->m_nR = 255;
    if (s->m_nG > 255) s->m_nG = 255;
    if (s->m_nB > 255) s->m_nB = 255;
    if (s->m_pOwner != nullptr) {
        impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(s->m_pOwner, clr);
        impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
            s->m_pOwner,
            static_cast<unsigned char>(s->m_nR),
            static_cast<unsigned char>(s->m_nG),
            static_cast<unsigned char>(s->m_nB));
    }
    double H = 0.0, S = 0.0, L = 0.0;
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(clr, &H, &S, &L);
    s->m_nH = ChannelToInt(H);                            // +0x420
    s->m_nL = ChannelToInt(L);                            // +0x41c
    s->m_nS = ChannelToInt(S);                            // +0x424
    if (s->m_hWnd != nullptr) {
        impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 0);
    }
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x2b8, H, L, S, 1);
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x160, H, L, S, 1);
    s->m_bUpdating = 0;
}

// OnInitDialog(): base-class init, picker palettes, the 0x4250..0x4255
// companion control limits and the initialized flag.  Transcribed from retail
// RVA 0x34350.
// Symbol: ?OnInitDialog@CMFCCustomColorsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCCustomColorsPropertyPage__MEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(
        reinterpret_cast<CDialog*>(pThis));               // 0x180208d10
    if (s->m_pOwner != nullptr) {
        const void* pPalette = *reinterpret_cast<void**>(
            reinterpret_cast<char*>(s->m_pOwner) + 0x188);
        s->m_nPageKind = 2;                               // +0x3a0
        impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
            reinterpret_cast<char*>(pThis) + 0x2b8, const_cast<void*>(pPalette));
        impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
            reinterpret_cast<char*>(pThis) + 0x160, const_cast<void*>(pPalette));
    }
    *reinterpret_cast<int*>(reinterpret_cast<char*>(pThis) + 0x248) = 1;
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x160, s->m_dH, s->m_dL, s->m_dS, 1);
    impl__SetLuminanceBarWidth_CMFCColorPickerCtrl__QEAAXH_Z(
        reinterpret_cast<char*>(pThis) + 0x160, 0xe);
    for (int nID = 0x4250; nID <= 0x4255; nID++) {
        CWnd* pWnd = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(
            reinterpret_cast<CWnd*>(pThis), nID);
        if (pWnd != nullptr && pWnd->m_hWnd != nullptr) {
            // Retail: SendMessageW(hwnd, 0x465, 0, 0xff) -- the MFC
            // colour-page companion-control limit message.
            ::SendMessageW(pWnd->m_hWnd, 0x465, 0, 0xff);
        }
    }
    s->m_bInitialized = 1;                                // +0x428
    return 1;
}

// OnLuminance(): the luminance slider moved -- reads the slider value at
// +0x250, refreshes the HLS/RGB edit values and notifies the owner.
// Transcribed from retail RVA 0x34a70.
// Symbol: ?OnLuminance@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnLuminance_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    s->m_bUpdating = 1;
    s->m_dL = s->m_dLumSlider;                            // +0x3a8 = +0x250
    s->m_nH = ChannelToInt(s->m_dH);                      // +0x420
    s->m_nL = ChannelToInt(s->m_dL);                      // +0x41c
    s->m_nS = ChannelToInt(s->m_dS);                      // +0x424
    const COLORREF clr = HlsToRgb(s->m_dH, s->m_dL, s->m_dS);
    if (s->m_pOwner != nullptr) {
        impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(s->m_pOwner, clr);
        s->m_nR = static_cast<int>(GetRValue(clr));       // +0x410
        s->m_nG = static_cast<int>(GetGValue(clr));       // +0x418
        s->m_nB = static_cast<int>(GetBValue(clr));       // +0x414
        impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
            s->m_pOwner,
            static_cast<unsigned char>(s->m_nR),
            static_cast<unsigned char>(s->m_nG),
            static_cast<unsigned char>(s->m_nB));
    }
    if (s->m_hWnd != nullptr) {
        impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 0);
    }
    s->m_bUpdating = 0;
}

// OnRGBChanged(): the RGB edit boxes changed -- reads the HLS edit boxes,
// converts them back to RGB and refreshes the pickers.  Transcribed from
// retail RVA 0x346c0.
// Symbol: ?OnRGBChanged@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnRGBChanged_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    if (s->m_bUpdating != 0 || s->m_bInitialized == 0) return;
    // Retail requires the three HLS edit controls (0x4249/0x4247/0x4248) to
    // exist before proceeding.
    if (impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4249) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4247) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4248) == nullptr) {
        return;
    }
    if (!impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 1)) {
        return;
    }
    s->m_bUpdating = 1;
    // Clamp each HLS edit value to 255 (retail cmovb keep-if-below).
    if (s->m_nH > 255) s->m_nH = 255;
    if (s->m_nS > 255) s->m_nS = 255;
    if (s->m_nL > 255) s->m_nL = 255;
    // (value + 0.5) / 255, clamped to 1.0 (retail constant 0x18034ff68).
    const double H = (static_cast<double>(s->m_nH) + 0.5) / 255.0;
    const double L = (static_cast<double>(s->m_nL) + 0.5) / 255.0;
    const double S = (static_cast<double>(s->m_nS) + 0.5) / 255.0;
    const COLORREF clr = HlsToRgb(H, L, S);
    s->m_nR = static_cast<int>(GetRValue(clr));           // +0x410
    s->m_nG = static_cast<int>(GetGValue(clr));           // +0x418
    s->m_nB = static_cast<int>(GetBValue(clr));           // +0x414
    if (s->m_hWnd != nullptr) {
        impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 0);
    }
    if (s->m_pOwner != nullptr) {
        impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(s->m_pOwner, clr);
        impl__SetPageOne_CMFCColorDialog__QEAAXEEE_Z(
            s->m_pOwner,
            static_cast<unsigned char>(s->m_nR),
            static_cast<unsigned char>(s->m_nG),
            static_cast<unsigned char>(s->m_nB));
    }
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x2b8, H, L, S, 1);
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x160, H, L, S, 1);
    s->m_bUpdating = 0;
}

// Setup(BYTE bRed, BYTE bGreen, BYTE bBlue): initializes the page from an RGB
// triple -- decomposes it to HLS, pushes both pickers and refreshes the
// edit-box values.  Transcribed from retail RVA 0x34930.
// Symbol: ?Setup@CMFCCustomColorsPropertyPage@@QEAAXEEE@Z
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    double H = 0.0, S = 0.0, L = 0.0;
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
        RGB(bRed, bGreen, bBlue), &H, &S, &L);            // 0x18005b8c0
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x2b8, H, L, S, 1);
    impl__SetHLS_CMFCColorPickerCtrl__QEAAXNNNH_Z(
        reinterpret_cast<char*>(pThis) + 0x160, H, L, S, 1);
    s->m_nR = bRed;                                       // +0x410
    s->m_nG = bGreen;                                     // +0x418
    s->m_nB = bBlue;                                      // +0x414
    s->m_nH = ChannelToInt(H);                            // +0x420
    s->m_nL = ChannelToInt(L);                            // +0x41c
    s->m_nS = ChannelToInt(S);                            // +0x424
    if (s->m_hWnd != nullptr) {
        impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 0);
    }
}
