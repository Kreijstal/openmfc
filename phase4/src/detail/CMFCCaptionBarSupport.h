#pragma once
// Shared internals of the former global_cmfccaptionbar.cpp translation unit.
// Definitions live in detail/CMFCCaptionBarSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccaptionbar {} } }
using namespace openmfc::detail::cmfccaptionbar;
// OpenMFC: CMFCCaptionBar exports.
//
// All 45 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// CMFCCaptionBar is only forward-declared in afxmfc.h, so `this` is taken as
// void* and the layout is mirrored with the file-local struct below.  The
// retail object is 0x6B8 bytes (see CreateObject at RVA 0x1fc10); the offsets
// were harvested from the retail bodies: the CWnd base block (m_hWnd at 0x40,
// the cached parent HWND at 0xa0), the tooltip-helper pointer at 0x408, the
// image-object slot at 0x410/0x418/0x420, the SetBitmap fields at 0x4f0/0x5b0,
// the alignment slots (0x5b4/0x610/0x654), the caption font at 0x5d8, the text
// rect / flag pair at 0x624/0x634, the button and gripper rects and flags
// (0x650..0x6b4) and the sizing members m_margin@0x68c / m_height@0x694 plus
// the border height used by OnNcCalcSize (0x684).
//
// Faithfully transcribed where the state is modeled: OnEraseBkgnd, OnGetFont,
// OnNcCalcSize, GetAlignment, EnableButton, OnSetFont (store half), OnMouseMove
// / OnMouseLeave / OnLButtonDown / OnLButtonUp (flag and rect maintenance plus
// the click WM_COMMAND path), PreTranslateMessage (tooltip relay and base
// tail-call), OnDrawBorder (rect inflate) and the element setters/removers
// (SetButton / SetButtonPressed / SetIcon / SetBitmap / Remove* / the tooltip
// setters).  The remaining bodies draw through the visual manager (0x180009774)
// or the tooltip manager (0x1803b25e8) and are kept as conservative
// type-correct versions.
//
// CSize-returning exports follow the retail MSVC hidden-return-slot ABI (`this`
// in %rcx, the CSize* slot in %rdx, slot written and returned); CRect-by-value
// parameters follow the MSVC x64 ABI observed in the retail bodies (the caller
// passes a pointer to a throwaway copy, exactly what GCC's ms_abi produces for
// a 16-byte struct).
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
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
    CWnd* pThis, MSG* pMsg);
// Forward declarations used by the tooltip setters.
extern "C" void MS_ABI impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(void* pThis);


namespace openmfc { namespace detail { namespace cmfccaptionbar {
struct CB {
    char    _pad000[0x40];
    HWND    m_hWnd;             // 0x040: CWnd::m_hWnd
    char    _pad048[0xa0 - 0x48];
    HWND    m_hParentWnd;       // 0x0a0: cached parent HWND (GetParent result)
    char    _pad0a8[0x3f8 - 0xa8];
    int     m_clrBackground;    // 0x3f8: background color (default -1)
    int     m_bTransparent;     // 0x400: transparent-background flag
    int     m_bDrawBackground;  // 0x404: draw-background flag
    void*   m_pToolTipWnd;      // 0x408: tooltip-helper CWnd* (m_hWnd at +0x40)
    void*   m_pImage;           // 0x410: image object pointer
    char    m_Image[8];         // 0x418: image object storage (RemoveBitmap)
    int     m_nImageCount;      // 0x420: image-object count
    char    _pad424[0x480 - 0x424];
    int     m_sizeImageCX;      // 0x480: cached image size.cx
    int     m_sizeImageCY;      // 0x484: cached image size.cy
    char    _pad488[0x4f0 - 0x488];
    int     m_nTransparent;     // 0x4f0: cached transparent color (SetBitmap)
    char    _pad4f4[0x5b0 - 0x4f4];
    int     m_nBitmapIndex;     // 0x5b0: bitmap index (SetBitmap)
    int     m_nButtonAlign;     // 0x5b4: button-element alignment
    CRect   m_rectImage;        // 0x5b8: image element rect
    void*   m_strImageTip;      // 0x5c8: CString (opaque) image tooltip
    void*   m_strImageTipDesc;  // 0x5d0: CString (opaque) image tooltip desc
    void*   m_hFont;            // 0x5d8: HFONT (OnGetFont / OnSetFont)
    void*   m_strText;          // 0x5e0: CString (opaque) caption text
    char    _pad5e8[0x5f0 - 0x5e8];
    void**  m_ppTextArray;      // 0x5f0: array of CString* (GetTextSize)
    int     m_nTextCount;       // 0x5f8: text-array count
    char    _pad5fc[0x610 - 0x5fc];
    int     m_nImageAlign;      // 0x610: image-element alignment
    char    _pad614[0x624 - 0x614];
    CRect   m_rectText;         // 0x624: text element rect (UpdateTooltips)
    int     m_nTextAligned;     // 0x634: text-aligned flag (UpdateTooltips)
    void*   m_strButtonText;    // 0x638: CString (opaque) button text
    void*   m_strButtonTip;     // 0x640: CString (opaque) button tooltip
    void*   m_strButtonTipDesc; // 0x648: CString (opaque) button tooltip desc
    int     m_nID;              // 0x650: button command id (WM_COMMAND)
    int     m_nTextAlign;       // 0x654: text-element alignment
    CRect   m_rectButton;       // 0x658: button element rect
    int     m_bButtonPressed;   // 0x668: button pressed flag
    int     m_bButtonHover;     // 0x66c: button hit-test result
    int     m_bButtonHighlight; // 0x670: button highlighted flag
    int     m_nButtonTransient; // 0x674: transient mouse flag (cleared on leave)
    int     m_bButtonEnabled;   // 0x678: button enabled flag (EnableButton)
    int     m_bIsClick;         // 0x67c: click-behavior flag (WM_COMMAND)
    int     m_clrBorder;        // 0x680: border color (OnDrawBorder)
    int     m_nBorderHeight;    // 0x684: border height (OnNcCalcSize)
    char    _pad688[0x68c - 0x688];
    int     m_margin;           // 0x68c: inflate margin (CheckRectangle)
    char    _pad690[0x694 - 0x690];
    int     m_height;           // 0x694: caption height (CalcFixedLayout)
    int     m_bGripperPressed;  // 0x698: gripper pressed flag (qword pair)
    int     m_bGripperHover;    // 0x69c: gripper hit-test result
    char    _pad6a0[0x6a4 - 0x6a0];
    CRect   m_rectGripper;      // 0x6a4: gripper element rect
    char    _pad6b4[0x6b8 - 0x6b4];
};
} } }


// Resolve the parent CWnd for the click WM_COMMAND relay (the same
// GetParent -> FromHandlePermanent chain the retail bodies run), caching the
// HWND at 0xa0 exactly like retail.
namespace openmfc { namespace detail { namespace cmfccaptionbar {
inline HWND ResolveParentHwnd(CB* s) {
    HWND hParent = s->m_hParentWnd;
    if (hParent == nullptr) {
        hParent = ::GetParent(s->m_hWnd);
        s->m_hParentWnd = hParent;
    }
    void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
    if (pParent == nullptr) return nullptr;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pParent) + 0x40);
}
} } }

// The retail bodies treat the CString members as an 8-byte slot holding a
// pointer to CStringData (nDataLength at ptr-0x10, matching afxstr.h).  These
// helpers operate on that representation; assignment goes through the
// OpenMFC CString so buffers carry a valid IAtlStringMgr for MSVC clients.
namespace openmfc { namespace detail { namespace cmfccaptionbar {
inline void AssignCStr_Cmfccaptionbar(void* pSlot, const wchar_t* psz) {
    if (pSlot == nullptr) return;
    CString* pStr = reinterpret_cast<CString*>(pSlot);
    *pStr = psz;   // null -> Empty()
}
} } }

namespace openmfc { namespace detail { namespace cmfccaptionbar {
inline void EmptyCStr(void* pSlot) {
    if (pSlot == nullptr) return;
    reinterpret_cast<CString*>(pSlot)->Empty();
}
} } }


//=============================================================================
// 5. CMFCCaptionBar (afxmfc.h only forward-declares the class)
//=============================================================================





//=============================================================================
// 1. Static factory
//=============================================================================


//=============================================================================
// 2. Simple getters / setters
//=============================================================================







//=============================================================================
// 3. Mouse interaction (transcribed)
//=============================================================================






//=============================================================================
// 4. Window message handlers
//=============================================================================














//=============================================================================
// 5. Element drawing (conservative -- draw through the visual manager)
//=============================================================================






//---------------------------------------------------------------------------
// 1.1 Element setters / removers
//---------------------------------------------------------------------------











//---------------------------------------------------------------------------
// 1.2 Tooltips
//---------------------------------------------------------------------------

