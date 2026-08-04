#pragma once
// Shared internals of the former global_cmfccolordialog.cpp translation unit.
// Definitions live in detail/CMFCColorDialogSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccolordialog {} } }
using namespace openmfc::detail::cmfccolordialog;
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


// The palette object at 0xbc0 is a 0x10-byte CGdiObject-like heap block
// { vtable, m_hObject } in retail.  The retail vtable (0x1802e0468) is not
// present here; OpenMFC-created holders leave vtable == nullptr so a foreign
// / uninitialized block can be recognised and never freed through.
namespace openmfc { namespace detail { namespace cmfccolordialog {
struct CPalHolder_Cmfccolordialog {
    void*   vtable;    // 0x00 (null = OpenMFC-created holder)
    HPALETTE m_hPal;   // 0x08 (m_hObject)
};
} } }


namespace openmfc { namespace detail { namespace cmfccolordialog {
struct CColorDlg {
    char    _pad000[0x40];
    HWND    m_hWnd;              // 0x040: CWnd::m_hWnd
    char    _pad048[0x170 - 0x48];
    void*   m_pCursorPicker;     // 0x170: picker sub-object (ctor-nulled)
    void*   m_pColorPicker;      // 0x178: CMFCColorPickerCtrl*
    void*   m_pSecondPicker;     // 0x180: second picker/list object
    CPalHolder_Cmfccolordialog* m_pPalette;      // 0x188: palette holder (from ctor arg)
    int     m_nCurrentColor;     // 0x190: current color (SetCurrentColor)
    int     m_nNewColor;         // 0x194: new color; bytes 0x194/0x195/0x196 = R/G/B
    HCURSOR m_hCursor;           // 0x198: eyedropper cursor
    int     m_bCustomPalette;    // 0x1a0: 1 = custom palette, 0 = system
    int     m_nTracking;         // 0x1a4: color-popup tracking active
    char    _pad1a8[0xdb0 - 0x1a8];   // 0x1a8 CMFCButton, 0xcc8 bar/wnd (opaque)
    char    m_picker[0x40];      // 0xdb0: CMFCColorPickerCtrl m_hWnd at +0x40
    char    _paddf0[0xdf0 - 0xdf0];
};
} } }















//-----------------------------------------------------------------------------
// CMFCColorDialog::SetPageOne
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CMFCColorDialog::SetPageTwo
//-----------------------------------------------------------------------------
