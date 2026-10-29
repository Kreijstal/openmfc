// CMFCCustomColorsPropertyPage — OpenMFC implementation.
// Sources: global_cmfccustomcolorspropertypage.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCustomColorsPropertyPageSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(
    void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);
extern "C" void* MS_ABI impl___0CMFCColorPickerCtrl__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCCustomColorsPropertyPage__QEAA_XZ(void* pThis);

static_assert(sizeof(CPropertyPage) == 0x158, "CPropertyPage base block ends at +0x158");
static_assert(sizeof(CColorsPage) == 0x430, "retail CreateObject requests 0x430 bytes");
static_assert(offsetof(CColorsPage, m_pOwner) == 0x158, "retail owner pointer at +0x158");
static_assert(offsetof(CColorsPage, m_dLumSlider) == 0x250, "retail picker member boundary at +0x250");
static_assert(offsetof(CColorsPage, m_bInitialized) == 0x428, "retail init flag at +0x428");

// CreateObject(): retail mfc140u RVA 0x34150 allocates 0x430 bytes with the
// MFC operator-new export and, when non-null, calls this class's constructor
// at entry RVA 0x341a0.
// Symbol: ?CreateObject@CMFCCustomColorsPropertyPage@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomColorsPropertyPage__SAPEAVCObject__XZ(void)
{
    void* p = impl___2_YAPEAX_K_Z(sizeof(CColorsPage));
    if (p == nullptr) return nullptr;
    return static_cast<CObject*>(impl___0CMFCCustomColorsPropertyPage__QEAA_XZ(p));
}
// DoDataExchange(CDataExchange* pDX): retail mfc140u RVA 0x34280 wires the two
// embedded pickers and six RGB/HLS edit boxes (the last DDX_Text is a tail
// call). OpenMFC also returns early for null this/pDX; retail has no such guard.
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
// OnColour(): retail mfc140u RVA 0x34b90 reads the COLORREF at +0x3c0,
// refreshes RGB/HLS values, invalidates the child at +0x1a0 and updates the
// owner. OpenMFC adds null-this/owner guards that the retail body lacks.
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
// OnDoubleClickedColor(): retail mfc140u RVA 0x34cd0 loads the owner and
// tail-jumps to CDialog::EndDialog(owner, 1), whose entry is mfc140u RVA
// 0x208b10. OpenMFC adds null-this/owner guards and calls the EndDialog thunk.
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
// OnHLSChanged(): retail mfc140.dll (ANSI twin) RVA 0x34800 reads HLS edit
// values and converts HLS to RGB; function bodies match mfc140u, whose RVA is
// unresolved by the supplied map. WARNING: the pre-existing OpenMFC body below
// instead follows the RGB-to-HLS path of retail OnRGBChanged and checks RGB
// control IDs. Kept unchanged because this body predates the current wave.
// Symbol: ?OnHLSChanged@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnHLSChanged_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    if (s->m_bUpdating != 0 || s->m_bInitialized == 0) return;
    // This pre-existing body checks RGB IDs 0x4244/0x4245/0x4246; retail
    // OnHLSChanged checks the HLS IDs 0x4249/0x4247/0x4248.
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
// OnInitDialog(): mfc140.dll (ANSI twin) RVA 0x34490 performs base init,
// picker palette setup, the 0x4250..0x4255 companion-control messages and sets
// the initialized flag; function bodies match mfc140u, whose RVA is unresolved
// by the supplied map. OpenMFC adds an owner-null guard (retail dereferences the
// owner) and only sets m_nPageKind when an owner exists.
// Symbol: ?OnInitDialog@CMFCCustomColorsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCCustomColorsPropertyPage__MEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    (void)impl__OnInitDialog_CDialog__UEAAHXZ(
        reinterpret_cast<CDialog*>(pThis));               // OpenMFC base-class thunk
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
            // The mfc140.dll IAT slot at RVA 0x2c5378 resolves to SendMessageA;
            // its mfc140u counterpart is Unicode SendMessageW. Use the generic
            // name so the UNICODE build selects the matching API.
            ::SendMessage(pWnd->m_hWnd, 0x465, 0, 0xff);
        }
    }
    s->m_bInitialized = 1;                                // +0x428
    return 1;
}
// OnLuminance(): mfc140.dll (ANSI twin) RVA 0x34bb0 reads the slider at +0x250,
// refreshes HLS/RGB values and updates the owner; function bodies match mfc140u,
// whose RVA is unresolved by the supplied map. OpenMFC updates the RGB fields
// only inside its owner-non-null branch, unlike retail.
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
// OnRGBChanged(): retail mfc140.dll (ANSI twin) RVA 0x345a0 reads RGB edit
// values and converts RGB to HLS; function bodies match mfc140u, whose RVA is
// unresolved by the supplied map. WARNING: the pre-existing OpenMFC body below
// instead follows retail OnHLSChanged's HLS-to-RGB path and checks HLS IDs.
// Kept unchanged because this body predates the current wave.
// Symbol: ?OnRGBChanged@CMFCCustomColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnRGBChanged_CMFCCustomColorsPropertyPage__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    if (s->m_bUpdating != 0 || s->m_bInitialized == 0) return;
    // This pre-existing body checks HLS IDs 0x4249/0x4247/0x4248; retail
    // OnRGBChanged checks the RGB IDs 0x4244/0x4245/0x4246.
    if (impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4249) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4247) == nullptr ||
        impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(reinterpret_cast<CWnd*>(pThis), 0x4248) == nullptr) {
        return;
    }
    if (!impl__UpdateData_CWnd__QEAAHH_Z(reinterpret_cast<CWnd*>(pThis), 1)) {
        return;
    }
    s->m_bUpdating = 1;
    // This pre-existing body uses signed >255 checks (negative inputs pass
    // through); retail OnHLSChanged uses an unsigned-below test and clamps
    // unsigned out-of-range values to 255.
    if (s->m_nH > 255) s->m_nH = 255;
    if (s->m_nS > 255) s->m_nS = 255;
    if (s->m_nL > 255) s->m_nL = 255;
    // This pre-existing HLS-to-RGB path divides (value + 0.5) by 255 but
    // omits retail's min(..., 1.0) clamp after the division.
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
// Setup(BYTE bRed, BYTE bGreen, BYTE bBlue): mfc140.dll (ANSI twin) RVA
// 0x34a70 decomposes RGB to HLS, updates both pickers and the edit values;
// function bodies match mfc140u, whose RVA is unresolved by the supplied map.
// OpenMFC adds a null-this guard.
// Symbol: ?Setup@CMFCCustomColorsPropertyPage@@QEAAXEEE@Z
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue)
{
    if (pThis == nullptr) return;
    CColorsPage* s = reinterpret_cast<CColorsPage*>(pThis);
    double H = 0.0, S = 0.0, L = 0.0;
    impl__RGBtoHSL_CDrawingManager__SAXKPEAN00_Z(
        RGB(bRed, bGreen, bBlue), &H, &S, &L);            // RGBtoHSL thunk
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
// Constructor. Retail mfc140u RVA 0x341a0 calls the CPropertyPage
// constructor with template 0x4243, caption 0 and PROPSHEETPAGE size 0x68,
// stores the derived vftable, constructs picker subobjects at +0x160/+0x2b8,
// then explicitly zeros +0x158, +0x410, +0x418, +0x420 and +0x428. OpenMFC
// has no modeled derived vftable, and its picker-constructor thunk currently
// only returns pThis; those are explicit deviations. OpenMFC zeroes the entire
// member tail before those no-op thunks, additionally clearing retail's other
// fields to avoid leaving indeterminate picker handles/state.
// Symbol: ??0CMFCCustomColorsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomColorsPropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, 0x4243, 0, 0x68);
    std::memset(static_cast<char*>(pThis) + 0x158, 0, sizeof(CColorsPage) - 0x158);
    impl___0CMFCColorPickerCtrl__QEAA_XZ(static_cast<char*>(pThis) + 0x160);
    impl___0CMFCColorPickerCtrl__QEAA_XZ(static_cast<char*>(pThis) + 0x2b8);
    return pThis;
}
