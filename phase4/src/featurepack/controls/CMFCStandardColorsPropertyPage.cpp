// CMFCStandardColorsPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// The standard-colours (hexagon) page of CMFCColorDialog.  Retail declaration:
// atlmfc/include/afxstandardcolorspropertypage.h:32 (14.51.36231) --
// CPropertyPage base, then m_pDialog, m_hexpicker, m_hexpicker_greyscale and the
// private m_nColorPickerOffset.  The class is not declared in include/openmfc,
// so `this` is taken as void* and the layout is pinned by the file-local shadow
// struct below.
//
// Every body here is transcribed from the retail export.  The disassembler's
// export map resolves these symbols in mfc140.dll (the ANSI twin); the RVAs are
// quoted for BOTH images.  Only the constructor, CreateObject, DoDataExchange
// and AdjustControlWidth are named in the mfc140u symbol map; for OnInitDialog,
// OnGreyscale, OnHexColor and OnSize the mfc140u RVA was located by the uniform
// -0xc80 displacement of this class's neighbouring exports and then confirmed
// by an instruction-for-instruction comparison against the mfc140 body
// (OnGreyscale 0x132e70, OnHexColor 0x132f10 and OnSize 0x132fa0 are also the
// pfn entries of the retail mfc140u message map at 0x1803109d0).
// OnDoubleClickedColor is code-folded in mfc140u; see its comment.
//
// Layout (all offsets read from the retail bodies):
//   +0x040  CWnd::m_hWnd                          (every CWnd-derived object)
//   +0x158  CMFCColorDialog* m_pDialog            (OnInitDialog / OnHexColor ...)
//   +0x160  CMFCColorPickerCtrl m_hexpicker       (DoDataExchange, ctor)
//   +0x2b8  CMFCColorPickerCtrl m_hexpicker_greyscale
//   +0x410  int m_nColorPickerOffset               (ctor stores 4)
//   sizeof == 0x418                                (CreateObject: new(0x418))
// Within an embedded CMFCColorPickerCtrl (retail afxcolorpickerctrl.h member
// order, and the OpenMFC shadow in detail/CMFCColorPickerCtrlSupport.h):
//   +0x040 m_hWnd, +0x0e8 m_COLORTYPE, +0x0f0 m_dblLum, +0x0f8 m_dblSat,
//   +0x100 m_dblHue, +0x108 m_colorNew.  sizeof == 0x158 (0x2b8 - 0x160).
// Within CMFCColorDialog (retail afxcolordialog.h member order: m_pPropSheet,
// m_pColourSheetOne, m_pColourSheetTwo, m_pPalette; the offsets are the ones
// OnGreyscale / OnInitDialog read):
//   +0x180 m_pColourSheetTwo (CMFCCustomColorsPropertyPage*), +0x188 m_pPalette.
// The shadow in detail/CMFCColorDialogSupport.h has the same two offsets but
// names them m_pSecondPicker / m_pPalette (and +0x178 m_pColorPicker, which
// retail declares as CMFCStandardColorsPropertyPage* m_pColourSheetOne).
//
// Known OpenMFC gaps that limit what these bodies achieve at run time:
//   * There is no MSVC-layout vftable for this class, so the constructor leaves
//     at +0x00 whatever the CPropertyPage constructor thunk installs (the same
//     treatment as the customize property pages, e.g. CMFCMousePropertyPage.cpp
//     deviation (1)).  A virtual OnInitDialog / DoDataExchange call through the
//     object therefore reaches CPropertyPage's entries, not the thunks below.
//   * The CMFCColorPickerCtrl constructor thunk (CMFCColorPickerCtrl.cpp) is
//     still a stub that only returns pThis, so the constructor zero-fills the
//     member block before calling it -- see the constructor comment.
//   * The featurepack/controls/MessageMaps.cpp map for this class is built on
//     g_emptyMsgEntries_Mfc07Msgmap (detail/Mfc07MsgmapSupport.cpp), so
//     OnGreyscale / OnHexColor / OnDoubleClickedColor / OnSize are not reached
//     through WM_COMMAND / WM_SIZE dispatch yet; they run only when called.
//     (Retail's map, dumped from mfc140u: BN_CLICKED 0x424c -> OnGreyscale,
//     BN_CLICKED 0x424e -> OnHexColor, BN_DOUBLECLICKED on both ->
//     OnDoubleClickedColor, WM_SIZE -> OnSize.)
//   * CDialog::OnInitDialog (detail/DlgcoreSupport.cpp) returns TRUE without
//     calling UpdateData(FALSE), so DoDataExchange's DDX_Control never runs
//     from OnInitDialog and the two pickers are never subclassed.  m_hWnd of
//     each picker therefore stays NULL.  IsWindow(NULL) and GetWindowRect(NULL)
//     fail harmlessly, but ::InvalidateRect(NULL, NULL, TRUE) -- which OnGreyscale
//     / OnHexColor issue unconditionally, as retail does -- invalidates and
//     redraws ALL windows; retail never hits that because its pickers are
//     subclassed by then.  The call is transcribed as-is, not guarded.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// Sibling / base thunks, signatures derived from their mangled names and
// matching the definitions cited on each line.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                  // detail/MemcoreSupport.cpp
extern "C" void MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(
    void* pDX, int nIDC, void* pv);                                                             // core/runtime/DdxExchange.cpp
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult);        // detail/DlgcoreSupport.cpp
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                    // detail/DlgcoreSupport.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(
    const CWnd* pThis, RECT* lpRect);                                                           // core/window/Thunks.cpp
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags);                                                                       // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__HLStoRGB_TWO_CDrawingManager__SAKNNN_Z(
    double H, double L, double S);                                                              // core/gdi/CDrawingManager.cpp
extern "C" void MS_ABI impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(
    void* pThis, unsigned long clr);                                                            // featurepack/controls/CMFCColorDialog.cpp
extern "C" void MS_ABI impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
    void* pThis, unsigned char bRed, unsigned char bGreen, unsigned char bBlue);                // featurepack/controls/CMFCCustomColorsPropertyPage.cpp
extern "C" void MS_ABI impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(
    void* pThis, unsigned long clr);                                                            // featurepack/controls/CMFCColorPickerCtrl.cpp
extern "C" void MS_ABI impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette);                                                               // featurepack/controls/CMFCColorPickerCtrl.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(
    void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);  // core/dialog/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCColorPickerCtrl__QEAA_XZ(void* pThis);                    // featurepack/controls/CMFCColorPickerCtrl.cpp
// Defined below; forward-declared so OnSize can call it.
extern "C" void MS_ABI impl__AdjustControlWidth_CMFCStandardColorsPropertyPage__AEAAXPEAVCMFCColorPickerCtrl__H_Z(
    void* pThis, void* pControl, int cx);

namespace {

// Embedded CMFCColorPickerCtrl -- only the members this page touches.
struct StdPagePicker {
    char     _pad000[0x40];
    HWND     m_hWnd;             // 0x040: CWnd::m_hWnd
    char     _pad048[0xe8 - 0x48];
    int      m_COLORTYPE;        // 0x0e8: CURRENT/LUMINANCE/PICKER/HEX(3)/HEX_GREYSCALE(4)
    char     _pad0ec[0xf0 - 0xec];
    double   m_dblLum;           // 0x0f0
    double   m_dblSat;           // 0x0f8
    double   m_dblHue;           // 0x100
    COLORREF m_colorNew;         // 0x108
    char     _pad10c[0x158 - 0x10c];
};
static_assert(sizeof(StdPagePicker) == 0x158, "CMFCColorPickerCtrl is 0x158 bytes (0x2b8 - 0x160)");
static_assert(offsetof(StdPagePicker, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(StdPagePicker, m_COLORTYPE) == 0xe8, "m_COLORTYPE");
static_assert(offsetof(StdPagePicker, m_dblLum) == 0xf0, "m_dblLum");
static_assert(offsetof(StdPagePicker, m_dblSat) == 0xf8, "m_dblSat");
static_assert(offsetof(StdPagePicker, m_dblHue) == 0x100, "m_dblHue");
static_assert(offsetof(StdPagePicker, m_colorNew) == 0x108, "m_colorNew");

struct StdColorsPage {
    char          _pad000[0x40];
    HWND          m_hWnd;                  // 0x040: CWnd::m_hWnd
    char          _pad048[0x158 - 0x48];   // rest of CPropertyPage
    void*         m_pDialog;               // 0x158: CMFCColorDialog*
    StdPagePicker m_hexpicker;             // 0x160
    StdPagePicker m_hexpicker_greyscale;   // 0x2b8
    int           m_nColorPickerOffset;    // 0x410
    char          _pad414[0x418 - 0x414];
};
static_assert(sizeof(StdColorsPage) == 0x418, "retail CreateObject allocates 0x418");
static_assert(offsetof(StdColorsPage, m_pDialog) == 0x158, "m_pDialog");
static_assert(offsetof(StdColorsPage, m_hexpicker) == 0x160, "m_hexpicker");
static_assert(offsetof(StdColorsPage, m_hexpicker_greyscale) == 0x2b8, "m_hexpicker_greyscale");
static_assert(offsetof(StdColorsPage, m_nColorPickerOffset) == 0x410, "m_nColorPickerOffset");
// The constructor placement-constructs OpenMFC's C++ CPropertyPage over
// [0, sizeof(CPropertyPage)) and then zero-fills from +0x158; that is only
// safe while the OpenMFC base ends exactly where retail's members begin.  The
// SetWindowPos / ScreenToClient thunks read CWnd::m_hWnd from the C++ class,
// so it must coincide with the +0x40 the shadows above use.
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "OpenMFC CWnd::m_hWnd matches the shadows' +0x40");

// The two CMFCColorDialog members these handlers read.
constexpr std::size_t kDlg_pColourSheetTwo = 0x180;   // CMFCCustomColorsPropertyPage*
constexpr std::size_t kDlg_pPalette        = 0x188;   // CPalette*

inline void* DlgMember(void* pDialog, std::size_t off) {
    return *reinterpret_cast<void**>(static_cast<char*>(pDialog) + off);
}

// Shared tail of OnGreyscale / OnHexColor: push the colour to the owner dialog
// and to page two, then mirror it into the OTHER picker of this page and
// repaint that picker.
void PropagateColor(StdColorsPage* s, COLORREF color, StdPagePicker* pOther) {
    const BYTE r = GetRValue(color);
    const BYTE g = GetGValue(color);
    const BYTE b = GetBValue(color);
    // Retail dereferences m_pDialog unconditionally; the null guard is an
    // OpenMFC deviation.  Neither constructor sets m_pDialog (the owning
    // CMFCColorDialog assigns it); OpenMFC's constructor zero-fills it, so a
    // handler run before that assignment skips the dialog instead of faulting.
    if (s->m_pDialog != nullptr) {
        impl__SetNewColor_CMFCColorDialog__QEAAXK_Z(s->m_pDialog, color);
        impl__Setup_CMFCCustomColorsPropertyPage__QEAAXEEE_Z(
            DlgMember(s->m_pDialog, kDlg_pColourSheetTwo), r, g, b);
    }
    impl__SetColor_CMFCColorPickerCtrl__QEAAXK_Z(pOther, RGB(r, g, b));
    ::InvalidateRect(pOther->m_hWnd, nullptr, TRUE);
}

} // namespace

// Constructor.  Retail RVA 0x133920 (mfc140) / 0x132ca0 (mfc140u), transcribed
// apart from the two deviations listed below:
//   CPropertyPage::CPropertyPage(IDD /*IDD_AFXBARRES_COLOR_PAGE_ONE, 0x4242*/, 0, 0x68);
//   <store class vftable>
//   m_hexpicker.CMFCColorPickerCtrl();             // +0x160, ctor 0x2bfa0 (mfc140u)
//   m_hexpicker_greyscale.CMFCColorPickerCtrl();   // +0x2b8
//   m_nColorPickerOffset = 4;                      // +0x410
// m_pDialog (+0x158) is not touched by retail.
// DEVIATIONS: (a) no vftable store -- OpenMFC has no MSVC vftable for this
// class, +0x00 keeps the CPropertyPage one (file header).  (b) The member
// block [0x158, 0x418) is zero-filled before the member constructors run.
// OpenMFC's CMFCColorPickerCtrl constructor thunk currently only returns
// pThis, so without this each picker would carry heap garbage in m_hWnd and
// in the GDI handle at picker+0x128 that SetPalette (called from OnInitDialog)
// hands to ::DeleteObject.  Retail's picker constructor (0x2bfa0 mfc140u)
// NULLs both: it runs CWnd::CWnd() (0x28a700), which delegates to
// CWnd::CWnd(HWND NULL) (0x28a730) storing the handle at +0x40, and it
// stores 0 at picker+0x128 itself.  The fill
// also leaves m_pDialog NULL, which the handlers' null guards test.  The
// picker vfptr stays NULL until that thunk is implemented.
// Symbol: ??0CMFCStandardColorsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCStandardColorsPropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, 0x4242, 0, 0x68);
    std::memset(static_cast<char*>(pThis) + offsetof(StdColorsPage, m_pDialog), 0,
                sizeof(StdColorsPage) - offsetof(StdColorsPage, m_pDialog));
    impl___0CMFCColorPickerCtrl__QEAA_XZ(&s->m_hexpicker);
    impl___0CMFCColorPickerCtrl__QEAA_XZ(&s->m_hexpicker_greyscale);
    s->m_nColorPickerOffset = 4;
    return pThis;
}
// AdjustControlWidth(CMFCColorPickerCtrl* pControl, int cx).  Transcribed from
// retail RVA 0x133c70 (mfc140) / 0x132ff0 (mfc140u):
//   if (!::IsWindow(pControl ? pControl->m_hWnd : NULL)) return;
//   CRect rect;  ::GetWindowRect(pControl->m_hWnd, &rect);  ScreenToClient(&rect);
//   pControl->SetWindowPos(NULL, m_nColorPickerOffset, rect.top,
//                          cx - 2 * m_nColorPickerOffset, rect.Height(),
//                          SWP_NOZORDER | SWP_NOACTIVATE /* 0x14 */);
// IAT slots resolved with iat.py (mfc140): 0x1802c5390 = IsWindow,
// 0x1802c5370 = GetWindowRect.  ScreenToClient is CWnd::ScreenToClient(LPRECT)
// on `this` (call at 0x133cce inside this function, mfc140); SetWindowPos is
// CWnd::SetWindowPos (call at 0x133d01, mfc140).
// Symbol: ?AdjustControlWidth@CMFCStandardColorsPropertyPage@@AEAAXPEAVCMFCColorPickerCtrl@@H@Z
extern "C" void MS_ABI impl__AdjustControlWidth_CMFCStandardColorsPropertyPage__AEAAXPEAVCMFCColorPickerCtrl__H_Z(
    void* pThis, void* pControl, int cx)
{
    StdPagePicker* ctl = static_cast<StdPagePicker*>(pControl);
    if (!::IsWindow(ctl != nullptr ? ctl->m_hWnd : nullptr)) return;
    if (pThis == nullptr) return;   // OpenMFC guard; retail uses `this` unchecked
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);

    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(ctl->m_hWnd, &rect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rect);
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
        static_cast<CWnd*>(pControl), nullptr,
        s->m_nColorPickerOffset, rect.top,
        cx - 2 * s->m_nColorPickerOffset, rect.bottom - rect.top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

// CreateObject.  Retail RVA 0x1338e0 (mfc140) / 0x132c60 (mfc140u), fully
// transcribed:
//   void* p = operator new(0x418);                 // 0x27f0 (mfc140u)
//   return p ? new (p) CMFCStandardColorsPropertyPage : NULL;   // ctor 0x132ca0
// The object carries the constructor's deviations (see above).  Note the
// CRuntimeClass descriptor in featurepack/controls/RuntimeClasses.cpp has a
// NULL m_pfnCreateObject, so RUNTIME_CLASS(...)->CreateObject() does not reach
// this export yet; only a direct call does.
// Symbol: ?CreateObject@CMFCStandardColorsPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCStandardColorsPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(StdColorsPage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCStandardColorsPropertyPage__QEAA_XZ(p);
}

// DoDataExchange(CDataExchange* pDX).  Transcribed from retail RVA 0x1339e0
// (mfc140) / 0x132d60 (mfc140u): two DDX_Control calls, the second a tail jump;
// the base DoDataExchange is not called.
//   DDX_Control(pDX, IDC_AFXBARRES_HEXPLACEHOLDER /*0x424e*/, m_hexpicker);
//   DDX_Control(pDX, IDC_AFXBARRES_GREYSCALEPLACEHOLDER /*0x424c*/, m_hexpicker_greyscale);
// Symbol: ?DoDataExchange@CMFCStandardColorsPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCStandardColorsPropertyPage__MEAAXPEAVCDataExchange___Z(
    void* pThis, void* pDX)
{
    if (pThis == nullptr) return;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x424e, &s->m_hexpicker);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, 0x424c, &s->m_hexpicker_greyscale);
}

// OnDoubleClickedColor(): m_pDialog->EndDialog(IDOK).  Transcribed from retail
// RVA 0x34e10 (mfc140) -- `mov 0x158(%rcx),%rcx; mov $1,%edx; jmp
// CDialog::EndDialog`.  In mfc140u the identical body sits at 0x34cd0, which
// that image's map names CMFCCustomColorsPropertyPage::OnDoubleClickedColor
// (the two are code-folded).  Retail passes m_pDialog without a null test;
// the thunk used here is itself null-safe.
// Symbol: ?OnDoubleClickedColor@CMFCStandardColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnDoubleClickedColor_CMFCStandardColorsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    impl__EndDialog_CDialog__QEAAXH_Z(static_cast<CDialog*>(s->m_pDialog), IDOK);
}

// OnGreyscale(): the greyscale strip changed.  Transcribed from retail RVA
// 0x133af0 (mfc140) / 0x132e70 (mfc140u):
//   COLORREF color = CDrawingManager::HLStoRGB_TWO(m_hexpicker_greyscale.m_dblHue,
//                        m_hexpicker_greyscale.m_dblLum, m_hexpicker_greyscale.m_dblSat);
//   m_pDialog->SetNewColor(color);
//   m_pDialog->m_pColourSheetTwo->Setup(GetRValue(color), GetGValue(color), GetBValue(color));
//   m_hexpicker.SetColor(RGB(r, g, b));
//   ::InvalidateRect(m_hexpicker.m_hWnd, NULL, TRUE);   // IAT 0x1802c5380 (mfc140), tail jump
// Symbol: ?OnGreyscale@CMFCStandardColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnGreyscale_CMFCStandardColorsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    const COLORREF color = static_cast<COLORREF>(impl__HLStoRGB_TWO_CDrawingManager__SAKNNN_Z(
        s->m_hexpicker_greyscale.m_dblHue,
        s->m_hexpicker_greyscale.m_dblLum,
        s->m_hexpicker_greyscale.m_dblSat));
    PropagateColor(s, color, &s->m_hexpicker);
}

// OnHexColor(): a hexagon cell was picked.  Transcribed from retail RVA
// 0x133b90 (mfc140) / 0x132f10 (mfc140u):
//   COLORREF color = m_hexpicker.m_colorNew;              // +0x268
//   m_pDialog->SetNewColor(color);
//   m_pDialog->m_pColourSheetTwo->Setup(GetRValue(color), GetGValue(color), GetBValue(color));
//   m_hexpicker_greyscale.SetColor(RGB(r, g, b));
//   ::InvalidateRect(m_hexpicker_greyscale.m_hWnd, NULL, TRUE);   // IAT 0x1802c5380 (mfc140)
// Symbol: ?OnHexColor@CMFCStandardColorsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnHexColor_CMFCStandardColorsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    PropagateColor(s, s->m_hexpicker.m_colorNew, &s->m_hexpicker_greyscale);
}

// OnInitDialog().  Transcribed from retail RVA 0x133a40 (mfc140) / 0x132dc0
// (mfc140u):
//   CDialog::OnInitDialog();                               // result ignored
//   m_hexpicker.SetPalette(m_pDialog->m_pPalette);
//   m_hexpicker.m_COLORTYPE = HEX;                         // 3
//   m_hexpicker_greyscale.SetPalette(m_pDialog->m_pPalette);
//   m_hexpicker_greyscale.m_COLORTYPE = HEX_GREYSCALE;     // 4
//   CRect rect; ::GetWindowRect(m_hexpicker.m_hWnd, &rect);  // IAT 0x1802c5370 (mfc140)
//   ScreenToClient(&rect);
//   m_nColorPickerOffset = rect.left;
//   return TRUE;
// Retail calls CDialog::OnInitDialog directly (0x206ec0 mfc140), not a
// CPropertyPage override.  OpenMFC's CDialog::OnInitDialog does not run
// UpdateData(FALSE), so the pickers are not subclassed here (see file header).
// Symbol: ?OnInitDialog@CMFCStandardColorsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCStandardColorsPropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return TRUE;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));

    // Retail reads m_pDialog->m_pPalette without a null test; the guard is an
    // OpenMFC deviation (stub constructor, see file header).
    void* pPalette = s->m_pDialog != nullptr ? DlgMember(s->m_pDialog, kDlg_pPalette) : nullptr;
    impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(&s->m_hexpicker, pPalette);
    s->m_hexpicker.m_COLORTYPE = 3;               // HEX
    impl__SetPalette_CMFCColorPickerCtrl__QEAAXPEAVCPalette___Z(&s->m_hexpicker_greyscale, pPalette);
    s->m_hexpicker_greyscale.m_COLORTYPE = 4;     // HEX_GREYSCALE

    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(s->m_hexpicker.m_hWnd, &rect);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(static_cast<const CWnd*>(pThis), &rect);
    s->m_nColorPickerOffset = rect.left;
    return TRUE;
}

// OnSize(UINT nType, int cx, int cy).  Transcribed from retail RVA 0x133c20
// (mfc140) / 0x132fa0 (mfc140u):
//   CWnd::Default();                     // the inline CWnd::OnSize
//   AdjustControlWidth(&m_hexpicker, cx);
//   AdjustControlWidth(&m_hexpicker_greyscale, cx);   // tail jump
// nType and cy are unused by retail.
// Symbol: ?OnSize@CMFCStandardColorsPropertyPage@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCStandardColorsPropertyPage__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy)
{
    (void)nType; (void)cy;
    if (pThis == nullptr) return;
    StdColorsPage* s = static_cast<StdColorsPage*>(pThis);
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
    impl__AdjustControlWidth_CMFCStandardColorsPropertyPage__AEAAXPEAVCMFCColorPickerCtrl__H_Z(
        pThis, &s->m_hexpicker, cx);
    impl__AdjustControlWidth_CMFCStandardColorsPropertyPage__AEAAXPEAVCMFCColorPickerCtrl__H_Z(
        pThis, &s->m_hexpicker_greyscale, cx);
}
