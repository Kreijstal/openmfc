#pragma once
// Shared internals of the former global_mfc-feature-34_impl.cpp translation unit.
// Definitions live in detail/MfcFeature34ImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcfeature34impl {} } }
using namespace openmfc::detail::mfcfeature34impl;
// OpenMFC: CMFCButton (afxbutton.h) remaining exports, wave1.
//
// All 30 symbols in this file were transcribed from the retail
// mfc140u.dll (14.51.36231, x64).  CMFCButton is a CWnd-derived owner-drawn
// button (size 0xB20 = 2848 bytes); the retail bodies address its members by
// raw byte offsets, so this file mirrors the class layout with a local
// struct `S_MfcFeature34Impl` (see below) instead of relying on the (minimal) header decl.
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


namespace openmfc { namespace detail { namespace mfcfeature34impl {
struct S_MfcFeature34Impl {
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
} } }


// CDC::m_hDC lives at offset 8 in the CDC object.
namespace openmfc { namespace detail { namespace mfcfeature34impl {
inline HDC hdc_of_MfcFeature34Impl(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}
} } }

// CMFCButton::UncheckRadioButtonsInGroup retail (RVA 0x1efb0, internal):
// walks the WS_GROUP radio siblings, removes WS_TABSTOP from each check-style
// sibling and unchecks it, then restores WS_TABSTOP on `this`.  Modeled here
// as a private helper (that symbol is not part of this wave's export list).
namespace openmfc { namespace detail { namespace mfcfeature34impl {
void UncheckRadioButtonsInGroupImpl_MfcFeature34Impl(S_MfcFeature34Impl* s);
} } }


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







//=============================================================================
// 2. State mutators
//=============================================================================







//=============================================================================
// 3. Mouse / focus handlers
//=============================================================================









//=============================================================================
// 4. Drawing virtuals
//=============================================================================








//=============================================================================
// 5. Owner-draw / control lifecycle
//=============================================================================



