#pragma once
// Shared internals of the former global_cmfccolorbar.cpp translation unit.
// Definitions live in detail/CMFCColorBarSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccolorbar {} } }
using namespace openmfc::detail::cmfccolorbar;
// OpenMFC: CMFCColorBar exports.
//
// All 39 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the layout is mirrored with the file-local struct below.  The retail
// object is 0x1508 bytes (classCMFCColorBar in global_mfc_toolbars_rtti.cpp);
// the named offsets were harvested from the retail bodies -- the CWnd base
// (m_hWnd 0x40, cached parent HWND 0xa0), the hot / highlighted indexes
// 0x1138/0x1140, the button CList at 0x1190 with its count at 0x11a0, the
// layout members at 0x1420..0x1454, the color members at 0x1468..0x14e8 and the
// three trailing object pointers 0x14f0/0x14f8/0x1500.
//
// Faithfully transcribed: GetExtraHeight, SetHorzMargin / SetVertMargin,
// ShowCommandMessageString (the WM_SETMESSAGESTRING relay), the
// OnPaletteChanged / OnQueryNewPalette repaint sequence, the OnKey Enter path,
// SetColor's early store path, SetDocumentColors' label / flag stores,
// GetHighlightedColor's index gate and the OnCreate / OnNcCalcSize / OnNcPaint /
// Rebuild guards.  The button objects (CMFCToolBarColorButton), the CList at
// 0x1190, the CArray at 0x1460 and the button-count virtual (vslot-0x2f0) are
// unmodeled and marked "TODO(clean-room)".
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

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cmfccolorbar {
struct CBar {
    char    _pad000[0x40];
    HWND    m_hWnd;            // 0x040: CWnd::m_hWnd
    char    _pad048[0xa0 - 0x48];
    HWND    m_hWndParent;      // 0x0a0: cached parent HWND (GetParent result)
    char    _pad0a8[0x10bc - 0xa8];
    int     m_nCaptionShow;    // 0x10bc: flag cleared by CreateControl
    char    _pad10c0[0x111c - 0x10c0];
    int     m_bAdjustGuard;    // 0x111c: AdjustLocations guard flag
    char    _pad1120[0x1138 - 0x1120];
    int     m_nHotIndex;       // 0x1138: hot button index (OnKey/SetColor/OnMouseLeave)
    char    _pad113c[0x1140 - 0x113c];
    int     m_nHighlightedIndex; // 0x1140: highlighted index (GetHighlightedColor)
    char    _pad1144[0x1190 - 0x1144];
    void*   m_pListButtons;    // 0x1190: CList-of-buttons head (opaque)
    char    _pad1198[0x11a0 - 0x1198];
    int     m_nListButtonCount;// 0x11a0: CList-of-buttons count
    char    _pad11a4[0x1420 - 0x11a4];
    int     m_nDefaultColumns; // 0x1420: default column count (Create)
    int     m_nColumnsOverride;// 0x1424: column override (Create)
    int     m_nRowsOverride;   // 0x1428: row override (Create)
    int     m_nMargin;         // 0x142c: layout margin (AdjustLocations)
    int     m_nTextMargin;     // 0x1430: text margin (AdjustLocations)
    int     m_nRowHeight;      // 0x1434: row height / label extra
    int     m_nMargin2;        // 0x1438: secondary margin (SetVertMargin)
    int     m_nMarginX;        // 0x143c: x margin (SetHorzMargin)
    int     m_bShowOtherButton;// 0x1440: other-button flag (EnableOtherButton)
    char    _pad1444[0x1448 - 0x1444];
    int     m_bGridSizeFlag;   // 0x1448: GetColorGridSize/OnNcPaint branch flag
    int     m_bResetAuto;      // 0x144c: reset-to-automatic flag (SetDocumentColors)
    int     m_nInteractionFlag;// 0x1450: mouse-handler gate flag
    int     m_nCommandId;      // 0x1454: command id (ShowCommandMessageString)
    char    _pad1458[0x1468 - 0x1458];   // 0x1458: opaque init object
    int     m_nColorCount;     // 0x1468: color count
    char    _pad146c[0x1498 - 0x146c];
    int     m_nButtonCount;    // 0x1498: button count (GetExtraHeight)
    char    _pad149c[0x14b8 - 0x149c];
    int     m_nColumns;        // 0x14b8: grid columns
    int     m_nGridRows;       // 0x14bc: grid rows
    char    _pad14c0[0x14d0 - 0x14c0];  // 0x14c0: CPalette member (opaque)
    int     m_nAutoColor;      // 0x14d0: current color (SetColor)
    unsigned int m_clrAuto;    // 0x14d4: auto button color (EnableAutomaticButton)
    void*   m_strAutoLabel;    // 0x14d8: CString auto-button label
    void*   m_strOtherLabel;   // 0x14e0: CString other-button label
    void*   m_strThirdLabel;   // 0x14e8: CString third label (SetDocumentColors)
    void*   m_pBtnAuto;        // 0x14f0: automatic-button object (opaque)
    void*   m_pBtnOther;       // 0x14f8: other-button object (opaque)
    void*   m_pMenuBar;        // 0x1500: child menu bar (opaque)
};
} } }


namespace openmfc { namespace detail { namespace cmfccolorbar {
struct alignas(8) CMap_KKCS_56Bytes {
    unsigned char data[56];
};
} } }

// The retail bodies treat the CString members as an 8-byte slot holding a
// pointer to CStringData (nDataLength at ptr-0x10, matching afxstr.h).  These
// helpers operate on that representation; assignment goes through the
// OpenMFC CString so buffers carry a valid IAtlStringMgr for MSVC clients.
namespace openmfc { namespace detail { namespace cmfccolorbar {
inline void AssignCStr_Cmfccolorbar(void* pSlot, const wchar_t* psz) {
    if (pSlot == nullptr) return;
    CString* pStr = reinterpret_cast<CString*>(pSlot);
    *pStr = psz;   // null -> Empty()
}
} } }

// CStringData::nDataLength is at (m_pszData - 0x10) -- the same read the
// retail bodies perform (e.g. `cmpl $0x0,-0x10(%rax)`).
namespace openmfc { namespace detail { namespace cmfccolorbar {
inline int CStrLen(const void* pszData) {
    if (pszData == nullptr) return 0;
    return *reinterpret_cast<const int*>(
        reinterpret_cast<const char*>(pszData) - 0x10);
}
} } }



//---------------------------------------------------------------------------
// 3.1 Construction
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// 3.2 Layout / sizing
//---------------------------------------------------------------------------






//---------------------------------------------------------------------------
// 3.3 Buttons / painting
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// 1. Color / selection accessors
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// 2. Margins / labels
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// 3. Message handlers
//---------------------------------------------------------------------------




















