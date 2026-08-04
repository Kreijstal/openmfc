#pragma once
// Shared internals of the former global_cmfcbutton.cpp translation unit.
// Definitions live in detail/CMFCButtonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcbutton {} } }
using namespace openmfc::detail::cmfcbutton;
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


namespace openmfc { namespace detail { namespace cmfcbutton {
struct S_Cmfcbutton {
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
} } }


// CDC::m_hDC lives at offset 8 in the CDC object.
namespace openmfc { namespace detail { namespace cmfcbutton {
inline HDC hdc_of_Cmfcbutton(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}
} } }

// First image object's handle slot (m_Images[0] + 0xa0); read back by the
// feature-34 OnGetImage export for wParam == 0.
namespace openmfc { namespace detail { namespace cmfcbutton {
inline void*& ImageHandleSlot(S_Cmfcbutton* s) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(s) + 0x1f8);
}
} } }

// CMFCButton::UncheckRadioButtonsInGroup retail (RVA 0x1efb0): walks the
// radio siblings, removes WS_TABSTOP from each check-style sibling and
// unchecks it, then restores WS_TABSTOP on `this`.  This is the same walk as
// the private helper in the feature-34 batch (which is a different TU), so it
// is reimplemented here as an exported symbol.
namespace openmfc { namespace detail { namespace cmfcbutton {
void UncheckRadioButtonsInGroupImpl_Cmfcbutton(S_Cmfcbutton* s);
} } }

// CMFCButton::PreTranslateMessage retail (RVA 0x1e560): resolves the parent
// CWnd and posts a WM_COMMAND notification carrying the button's dialog ID.
namespace openmfc { namespace detail { namespace cmfcbutton {
inline void SendButtonCommandToParent(void* pThis) {
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->m_hWnd == nullptr) return;
    HWND hParent = ::GetParent(s->m_hWnd);
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (pParent == nullptr) return;
    HWND hwndParent = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pParent) + 0x40);
    WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
    ::SendMessageW(hwndParent, WM_COMMAND, MAKEWPARAM(wId, 0),
                   reinterpret_cast<LPARAM>(s->m_hWnd));
}
} } }


//=============================================================================
// 1. Message handlers
//=============================================================================











//=============================================================================
// 2. Image setter family
//=============================================================================











//=============================================================================
// 3. Cursor / std image
//=============================================================================




//=============================================================================
// 4. Sizing / radio group / accessibility
//=============================================================================



