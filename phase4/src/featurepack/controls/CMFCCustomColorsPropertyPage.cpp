// CMFCCustomColorsPropertyPage — OpenMFC implementation.
// Sources: global_cmfccustomcolorspropertypage.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCustomColorsPropertyPageSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

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
    const COLORREF clr = HlsToRgb_Cmfccustomcolorspropertypage(s->m_dH, s->m_dL, s->m_dS);
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
    const COLORREF clr = HlsToRgb_Cmfccustomcolorspropertypage(s->m_dH, s->m_dL, s->m_dS);
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
    const COLORREF clr = HlsToRgb_Cmfccustomcolorspropertypage(H, L, S);
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
// Symbol: ??0CMFCCustomColorsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomColorsPropertyPage__QEAA_XZ(void* pThis) {
    return pThis;
}
