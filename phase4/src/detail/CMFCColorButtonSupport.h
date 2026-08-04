#pragma once
// Shared internals of the former global_cmfccolorbutton.cpp translation unit.
// Definitions live in detail/CMFCColorButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccolorbutton {} } }
using namespace openmfc::detail::cmfccolorbutton;
// OpenMFC: CMFCColorButton exports.
//
// All 20 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers (it only appears in the
// afxmfc.h CMFCButton forward declarations), so `this` is taken as void* and
// the layout is mirrored with the file-local struct below.  The retail object
// is 0xBC8 bytes (classCMFCColorButton in global_popupmenu_button_rtti.cpp);
// its base is CMFCButton, 0xB20 bytes (see global_cmfcbutton.cpp).  The stub
// constructors (manual_small_stub_implementations.cpp) leave the object state
// uninitialized, so every container / pointer access is guarded and a zeroed
// object behaves like a fresh retail object.
//
// Faithfully transcribed: SetColor, SetCurrentColor, UpdateColor, OnGetDlgCode,
// OnSysColorChange, SizeToContent (through the CMFCButton base), OnDrawFocusRect,
// OnFillBackground, OnMouseMove, RebuildPalette, the two Enable*Button setters
// and the pre-closing branch of OnShowColorPopup.  The visual-manager singleton
// (0x18006a790 / 0x18006b1c0), the module flags (0x1803be1a8) and the 0x2ed8-byte
// CMFCColorPopup window (0x180024730) are unmodeled and marked
// "TODO(clean-room)".
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
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSysColorChange_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
    CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy,
    unsigned int nFlags);
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly);
extern "C" void MS_ABI impl__OnMouseMove_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point);
extern "C" void MS_ABI impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient);
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect);
extern "C" int MS_ABI impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
    void* pPalette, void* arColors);
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCColorButton__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* lpszLabel, unsigned long clr, int bShowBorder);
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorButton__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bAuto, int bShowBorder);
// Forward declarations of exports defined later in this file (cross-referenced
// by the button message handlers below).
extern "C" void MS_ABI impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette);
extern "C" void MS_ABI impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(
    void* pThis);


// The palette object at 0xbc0 is a 0x10-byte CGdiObject-like heap block
// { vtable, m_hObject } in retail.  The retail vtable (0x1802e0468) is not
// present here; OpenMFC-created holders leave vtable == nullptr so a foreign
// / uninitialized block can be recognised and never freed through.
namespace openmfc { namespace detail { namespace cmfccolorbutton {
struct CPalHolder_Cmfccolorbutton {
    void*   vtable;    // 0x00 (null = OpenMFC-created holder)
    HPALETTE m_hPal;   // 0x08 (m_hObject)
};
} } }


namespace openmfc { namespace detail { namespace cmfccolorbutton {
struct CColorBtn {
    char    _pad000[0x40];
    HWND    m_hWnd;              // 0x040: CWnd::m_hWnd
    char    _pad048[0xe8 - 0x48];
    int     m_nStyle;            // 0x0e8: CMFCButton style (2 = pushed)
    char    _padec[0x11c - 0xec];
    int     m_bClickStarted;     // 0x11c: click-in-progress flag
    int     m_bCapture;          // 0x120: mouse-capture flag
    char    _pad124[0xb20 - 0x124];
    int     m_nCommandId;        // 0xb20: command id (OnShowColorPopup arg)
    int     m_bShowPopupFlag;    // 0xb24: popup-behavior flag
    int     m_nColor;            // 0xb28: current color (SetColor stores here)
    int     m_clrAutomaticButton;// 0xb2c: automatic-button color
    char    _padb30[0x28];       // 0xb30: CArray<COLORREF,COLORREF> (opaque here;
                                 //        SetPalette touches the retail layout:
                                 //        m_pData@+0x8, m_nSize@+0x10, m_nMaxSize@+0x18)
    char    _padb58[0x30];       // 0xb58: CList<COLORREF,COLORREF> (SetDocumentColors
                                 //        label path only -- the list sync is not modeled)
    char    _padb88[0xb90 - 0xb88];
    int     m_nPopupArg;         // 0xb90: arg passed to the popup object
    void*   m_pColorPopup;       // 0xb98: active color-popup window object
    void*   m_strOtherLabel;     // 0xba0: CString other-button label
    void*   m_strDocLabel;       // 0xba8: CString document-colors label
    void*   m_strAutoLabel;      // 0xbb0: CString automatic-button label
    int     m_bOtherButton;      // 0xbb8: other-button flag
    char    _padbbc[0xbc0 - 0xbbc];
    CPalHolder_Cmfccolorbutton* m_pPaletteObj;   // 0xbc0: palette holder (RebuildPalette/OnDraw)
};
} } }


// The retail bodies treat the CString members as an 8-byte slot holding a
// pointer to CStringData (nDataLength at ptr-0x10, matching afxstr.h).  These
// helpers operate on that representation; assignment goes through the
// OpenMFC CString so buffers carry a valid IAtlStringMgr for MSVC clients.
namespace openmfc { namespace detail { namespace cmfccolorbutton {
inline void AssignCStr_Cmfccolorbutton(void* pSlot, const wchar_t* psz) {
    if (pSlot == nullptr) return;
    CString* pStr = reinterpret_cast<CString*>(pSlot);
    *pStr = psz;   // null -> Empty()
}
} } }

// CDC::m_hDC lives at offset 8 in the CDC object.
namespace openmfc { namespace detail { namespace cmfccolorbutton {
inline HDC hdc_of_Cmfccolorbutton(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}
} } }
















// OnDrawBorder and OnDrawFocusRect forward through the base CMFCButton
// helpers; OnInitControl is below with the popup/profile machinery.






