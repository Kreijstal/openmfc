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

namespace {

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

static_assert(sizeof(CB) == 0x6b8, "CMFCCaptionBar mirror must be 0x6B8 bytes");
static_assert(offsetof(CB, m_hWnd) == 0x40, "CMFCCaptionBar m_hWnd @0x40");
static_assert(offsetof(CB, m_nTransparent) == 0x4f0, "CMFCCaptionBar m_nTransparent @0x4f0");
static_assert(offsetof(CB, m_nBitmapIndex) == 0x5b0, "CMFCCaptionBar m_nBitmapIndex @0x5b0");
static_assert(offsetof(CB, m_nButtonAlign) == 0x5b4, "CMFCCaptionBar m_nButtonAlign @0x5b4");
static_assert(offsetof(CB, m_rectText) == 0x624, "CMFCCaptionBar m_rectText @0x624");
static_assert(offsetof(CB, m_nTextAligned) == 0x634, "CMFCCaptionBar m_nTextAligned @0x634");
static_assert(offsetof(CB, m_nID) == 0x650, "CMFCCaptionBar m_nID @0x650");
static_assert(offsetof(CB, m_nTextAlign) == 0x654, "CMFCCaptionBar m_nTextAlign @0x654");
static_assert(offsetof(CB, m_bButtonHighlight) == 0x670, "CMFCCaptionBar m_bButtonHighlight @0x670");
static_assert(offsetof(CB, m_bIsClick) == 0x67c, "CMFCCaptionBar m_bIsClick @0x67c");
static_assert(offsetof(CB, m_rectGripper) == 0x6a4, "CMFCCaptionBar m_rectGripper @0x6a4");

// Resolve the parent CWnd for the click WM_COMMAND relay (the same
// GetParent -> FromHandlePermanent chain the retail bodies run), caching the
// HWND at 0xa0 exactly like retail.
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

// The retail bodies treat the CString members as an 8-byte slot holding a
// pointer to CStringData (nDataLength at ptr-0x10, matching afxstr.h).  These
// helpers operate on that representation; assignment goes through the
// OpenMFC CString so buffers carry a valid IAtlStringMgr for MSVC clients.
inline void AssignCStr(void* pSlot, const wchar_t* psz) {
    if (pSlot == nullptr) return;
    CString* pStr = reinterpret_cast<CString*>(pSlot);
    *pStr = psz;   // null -> Empty()
}

inline void EmptyCStr(void* pSlot) {
    if (pSlot == nullptr) return;
    reinterpret_cast<CString*>(pSlot)->Empty();
}

} // namespace

//=============================================================================
// 5. CMFCCaptionBar (afxmfc.h only forward-declares the class)
//=============================================================================

// AdjustRectToMargin(CRect&, const CRect&, int, int): clamps the caption
// rect into the screen rect inflated by nMargin and, when bFixSize is set,
// keeps the rect size fixed by re-anchoring.  Fully transcribed from retail
// RVA 0x21fd0 (including the retail register-level clamp order).
// Symbol: ?AdjustRectToMargin@CMFCCaptionBar@@IEAAXAEAVCRect@@AEBV2@HH@Z
extern "C" void MS_ABI impl__AdjustRectToMargin_CMFCCaptionBar__IEAAXAEAVCRect__AEBV2_HH_Z(
    void* pThis, CRect* rect, const CRect* rectScreen, int nMargin, int bFixSize)
{
    if (pThis == nullptr || rect == nullptr || rectScreen == nullptr) return;
    const int leftOrig = rect->left;
    const int topOrig = rect->top;

    int n = rectScreen->left + nMargin;
    int rr = rectScreen->right - nMargin;
    bool f1, f2;
    if (rect->left < n) {
        rect->left = n;
        f1 = true;
    } else {
        f1 = false;
        n = leftOrig;
    }
    if (rect->top > rr) {
        rect->top = rr;
        f2 = true;
    } else {
        f2 = false;
        rr = topOrig;
    }
    if (bFixSize != 0) {
        const int d = topOrig - leftOrig;
        if (f1) {
            rect->top = d + n;
        } else if (f2) {
            rect->left = rr - d;
        }
    }
}

// CalcFixedLayout(BOOL, BOOL): reports the fixed layout size.  Retail
// (RVA 0x21070) makes a vtable+0x430 side-effect call (not modeled) and
// returns CSize(0x7fff, m_height@0x694).  CSize is returned through the
// hidden return slot.
// Symbol: ?CalcFixedLayout@CMFCCaptionBar@@MEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCCaptionBar__MEAA_AVCSize__HH_Z(
    void* pThis, CSize* pRet, int /*nStretch*/, int /*nCalcBorders*/)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    // TODO(clean-room): retail vtable+0x430 helper call not modeled.
    pRet->cx = 0x7fff;
    pRet->cy = reinterpret_cast<CB*>(pThis)->m_height;
    return pRet;
}

// CheckRectangle(CRect&, const CRect&, BOOL): pins the caption rect's top-left
// corner inside the screen rect inflated by m_margin (0x68c).  The retail
// body (RVA 0x21ef0) always returns FALSE; the clamp order is reproduced
// structurally (L/T = inflated left/top edge).
// Symbol: ?CheckRectangle@CMFCCaptionBar@@IEAAHAEAVCRect@@AEBV2@H@Z
extern "C" int MS_ABI impl__CheckRectangle_CMFCCaptionBar__IEAAHAEAVCRect__AEBV2_H_Z(
    void* pThis, CRect* rect, const CRect* rectScreen, int bBottomGripper)
{
    if (pThis == nullptr || rect == nullptr || rectScreen == nullptr) return 0;
    if (::IsRectEmpty(reinterpret_cast<const RECT*>(rect)) ||
        ::IsRectEmpty(reinterpret_cast<const RECT*>(rectScreen))) return 0;
    const int m = reinterpret_cast<CB*>(pThis)->m_margin;
    RECT i = {rectScreen->left, rectScreen->top, rectScreen->right, rectScreen->bottom};
    ::InflateRect(&i, m, m);
    const int L = i.left;
    const int T = i.top;

    int cl;
    if (rect->left >= L && rect->left <= T) {
        rect->left = T;
        cl = T;
    } else {
        cl = rect->left;
    }
    if (rect->top >= L && rect->top <= T) rect->top = L;
    if (rect->top >= L && rect->top <= T) rect->top = cl;

    if (cl > L || rect->top < T) {
        if (bBottomGripper == 0) {
            if (rect->top > L) return 0;
            rect->left = rect->top;
        } else {
            if (rect->left >= T) rect->left = L;
        }
        return 0;
    }
    if (bBottomGripper == 0) {
        rect->left = T;
        if (rect->top > L) return 0;
        rect->left = rect->top;
    } else {
        rect->top = L;
        if (rect->left >= T) rect->left = L;
    }
    return 0;
}

// Create(DWORD, CWnd*, UINT, int, int): factory.  The retail body
// (RVA 0x20050) runs CreateEx through a vtable slot, IsKindOf checks against
// the frame/dock runtime classes, and builds the caption elements; none of
// that is modeled, so the call conservatively fails.
// Symbol: ?Create@CMFCCaptionBar@@QEAAHKPEAVCWnd@@IHH@Z
extern "C" int MS_ABI impl__Create_CMFCCaptionBar__QEAAHKPEAVCWnd__IHH_Z(
    void* pThis, unsigned long /*dwStyle*/, void* /*pParentWnd*/,
    unsigned int /*nID*/, int /*nHeight*/, int /*bBottomGripper*/)
{
    if (pThis == nullptr) return FALSE;
    // TODO(clean-room): CreateEx + IsKindOf + caption-element construction not
    // modeled.
    return FALSE;
}

//=============================================================================
// 1. Static factory
//=============================================================================

// CreateObject(): CRuntimeClass factory.  Retail (RVA 0x1fc10) allocates a
// 0x6B8-byte CMFCCaptionBar (operator new at 0x1800027f0) and runs the
// constructor (RVA 0x1fc50) which installs the retail vftable (0x1802df728)
// and the tooltip-manager hook.  The class is opaque in OpenMFC -- no
// constructor, vftable or member state is modeled -- so no valid object can
// be produced and the factory conservatively fails.
// Symbol: ?CreateObject@CMFCCaptionBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCCaptionBar__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): transcribed partially -- retail allocates 0x6B8 bytes
    // and runs the CMFCCaptionBar constructor (RVA 0x1fc50, retail vftable at
    // 0x1802df728); the opaque class has no modeled constructor or vftable,
    // so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}

//=============================================================================
// 2. Simple getters / setters
//=============================================================================

// EnableButton(BOOL): enables/disables the caption button.  Retail
// (RVA 0x21120) stores bEnable into the m_bButtonEnabled slot (0x678) and,
// while a window exists, runs the vslot-0x430 layout virtual and refreshes
// the button rect (0x658).
// Symbol: ?EnableButton@CMFCCaptionBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableButton_CMFCCaptionBar__QEAAXH_Z(
    void* pThis, int bEnable)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_bButtonEnabled = bEnable;
    if (s->m_hWnd != nullptr) {
        // TODO(clean-room): retail also runs the vslot-0x430 layout virtual
        // before invalidating.
        ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}

// GetAlignment(BarElement): returns the stored alignment for the text (0),
// image (1) or button (2) element; every other id returns 0.  Fully
// transcribed from retail RVA 0x21720.
// Symbol: ?GetAlignment@CMFCCaptionBar@@QEAA?AW4BarElementAlignment@1@W4BarElement@1@@Z
extern "C" int MS_ABI impl__GetAlignment_CMFCCaptionBar__QEAA_AW4BarElementAlignment_1_W4BarElement_1__Z(
    void* pThis, int nElement)
{
    if (pThis == nullptr) return 0;
    CB* s = reinterpret_cast<CB*>(pThis);
    switch (nElement) {
    case 0: return s->m_nTextAlign;    // 0x654
    case 1: return s->m_nImageAlign;   // 0x610
    case 2: return s->m_nButtonAlign;  // 0x5b4
    default: return 0;
    }
}

// GetImageSize(): const accessor returning the image size through the hidden
// CSize slot.  Retail (RVA 0x22040): when the image count (0x420) is > 0 the
// cached size (0x480) is returned for a null image pointer (and the body fails
// fast otherwise); when the count is 0 and an image object (0x410) exists the
// size is resolved via GetIconInfo/GetObjectW; with neither, CSize(0,0) is
// returned.  The image-object state is unmodeled, so the zero terminal is
// produced.
// Symbol: ?GetImageSize@CMFCCaptionBar@@IEBA?AVCSize@@XZ
extern "C" CSize* MS_ABI impl__GetImageSize_CMFCCaptionBar__IEBA_AVCSize__XZ(
    void* pThis, CSize* pRet)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    CB* s = reinterpret_cast<CB*>(pThis);
    if (s->m_nImageCount > 0) {
        if (s->m_pImage == nullptr) {
            pRet->cx = s->m_sizeImageCX;
            pRet->cy = s->m_sizeImageCY;
        }
        // TODO(clean-room): retail fails fast (0x180227720) when the count and
        // the image pointer are both set; not modeled.
        return pRet;
    }
    if (s->m_pImage != nullptr) {
        // TODO(clean-room): retail resolves the size from the image object
        // through GetIconInfo/GetObjectW; the image object is not modeled.
        return pRet;
    }
    return pRet;
}

// GetTextSize(CDC*, const CString&): measures the caption text size.  Retail
// (RVA 0x22a10): for a single line (m_nTextCount@0x5f8 == 1) it runs
// GetTextExtentPoint32W with the caption font (0x5d8); otherwise it scans the
// text-array (0x5f0, count 0x5f8) and sums the per-line heights.  The text
// state is unmodeled (count 0), so CSize(0,0) -- the retail terminal for an
// empty array -- is returned.
// Symbol: ?GetTextSize@CMFCCaptionBar@@MEAA?AVCSize@@PEAVCDC@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" CSize* MS_ABI impl__GetTextSize_CMFCCaptionBar__MEAA_AVCSize__PEAVCDC__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CSize* pRet, CDC* /*pDC*/, const CString& /*strText*/)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    // TODO(clean-room): transcribed partially -- the single-line path
    // (GetTextExtentPoint32W on strText with the caption font at 0x5d8) and
    // the multi-line text-array scan (0x5f0 / 0x5f8) depend on unmodeled text
    // state; with the count unmodeled (0) retail also returns CSize(0,0).
    return pRet;
}

// OnGetFont(): returns the caption font handle.  Fully transcribed from
// retail RVA 0x21710 (single load of the 0x5d8 slot).
// Symbol: ?OnGetFont@CMFCCaptionBar@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CMFCCaptionBar__IEAAPEAUHFONT____XZ(
    void* pThis)
{
    if (pThis == nullptr) return nullptr;
    return reinterpret_cast<CB*>(pThis)->m_hFont;
}

// OnSetFont(CFont*, BOOL): stores the font's CFont::m_hObject handle (offset
// 8) into the m_hFont slot (0x5d8), then tail-calls the base dispatch.
// Transcribed from retail RVA 0x216e0.
// Symbol: ?OnSetFont@CMFCCaptionBar@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCCaptionBar__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int /*bRedraw*/)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_hFont = (pFont != nullptr)
        ? *reinterpret_cast<void**>(reinterpret_cast<char*>(pFont) + 8)
        : nullptr;
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch here;
    // the base CWnd::OnSetFont implementation is not modeled.
}

//=============================================================================
// 3. Mouse interaction (transcribed)
//=============================================================================

// OnMouseMove(UINT, CPoint): hit-tests the button rect (0x658, when a button
// id is set and the button is enabled) and the gripper rect (0x6a4), tracks
// the hover flags and refreshes the matching rect whenever the hit-test
// result changes.  Transcribed from retail RVA 0x222c0.
// Symbol: ?OnMouseMove@CMFCCaptionBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCCaptionBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, CPoint point)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    // TODO(clean-room): retail first runs the base CWnd::OnMouseMove dispatch
    // (0x18009fce0); not modeled.
    if (s->m_nID != 0 && s->m_bButtonEnabled != 0) {
        const int hit = ::PtInRect(reinterpret_cast<const RECT*>(&s->m_rectButton), point);
        if (hit != s->m_bButtonHover) {
            s->m_bButtonHover = hit;
            s->m_bButtonPressed = ((nFlags & 1) != 0 && hit != 0) ? 1 : 0;
            if (s->m_hWnd != nullptr) {
                ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
                ::UpdateWindow(s->m_hWnd);
            }
        }
    }
    if (!::IsRectEmpty(reinterpret_cast<const RECT*>(&s->m_rectGripper))) {
        const int hit = ::PtInRect(reinterpret_cast<const RECT*>(&s->m_rectGripper), point);
        if (hit != s->m_bGripperHover) {
            s->m_bGripperHover = hit;
            s->m_bGripperPressed = ((nFlags & 1) != 0 && hit != 0) ? 1 : 0;
            if (s->m_hWnd != nullptr) {
                ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectGripper), TRUE);
                ::UpdateWindow(s->m_hWnd);
            }
        }
    }
}

// OnMouseLeave(): clears the transient mouse flag, releases the button and
// gripper hover/pressed pairs and refreshes the matching rects.  Transcribed
// from retail RVA 0x223e0 (the gripper pair at 0x698 is cleared as one qword,
// matching the retail 0x18002245c store).
// Symbol: ?OnMouseLeave@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_nButtonTransient = 0;
    if (s->m_bButtonPressed != 0 || s->m_bButtonHover != 0) {
        s->m_bButtonPressed = 0;
        s->m_bButtonHover = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
    }
    if (s->m_bGripperPressed != 0 || s->m_bGripperHover != 0) {
        s->m_bGripperPressed = 0;
        s->m_bGripperHover = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectGripper), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
    }
}

// OnLButtonDown(UINT, CPoint): marks the button pressed when the hit-test
// agrees and the button is enabled, forwards the click to the parent
// (WM_COMMAND, m_nID) when the click-behavior flag is set, and tracks the
// gripper press.  Transcribed from retail RVA 0x220f0.
// Symbol: ?OnLButtonDown@CMFCCaptionBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCCaptionBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    // TODO(clean-room): retail first runs the base CWnd::OnLButtonDown
    // dispatch (0x18009f810); not modeled.
    if (s->m_nID != 0 && s->m_bButtonEnabled != 0 && s->m_bButtonHover != 0) {
        s->m_bButtonPressed = 1;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
        if (s->m_bIsClick != 0) {
            HWND hwndParent = ResolveParentHwnd(s);
            if (hwndParent != nullptr) {
                ::SendMessageW(hwndParent, WM_COMMAND,
                               static_cast<WPARAM>(s->m_nID), 0);
            }
        }
    }
    if (s->m_bGripperHover != 0) {
        s->m_bGripperPressed = 1;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectGripper), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
    }
}

// OnLButtonUp(UINT, CPoint): releases the button press and forwards the click
// WM_COMMAND to the parent unless the click-behavior flag consumes it;
// otherwise releases a pending gripper press and notifies the vslot-0x458
// virtual.  Transcribed from retail RVA 0x221c0.
// Symbol: ?OnLButtonUp@CMFCCaptionBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCCaptionBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    // TODO(clean-room): retail first runs the base CWnd::OnLButtonUp dispatch
    // (0x18009fa80); not modeled.
    if (s->m_bButtonPressed != 0) {
        s->m_bButtonPressed = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
        if (s->m_bIsClick == 0 && s->m_nID != 0) {
            HWND hwndParent = ResolveParentHwnd(s);
            if (hwndParent != nullptr) {
                ::SendMessageW(hwndParent, WM_COMMAND,
                               static_cast<WPARAM>(s->m_nID), 0);
            }
        }
    } else if (s->m_bGripperPressed != 0) {
        s->m_bGripperPressed = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, reinterpret_cast<const RECT*>(&s->m_rectGripper), TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
        // TODO(clean-room): retail also invokes the vslot-0x458 virtual with
        // (0, 0, 0); not modeled.
    }
}

// OnRButtonUp(UINT, CPoint): when the context-menu flag global (0x1803be35c)
// is clear the retail body resolves the parent and runs the vslot-0x4a0
// context-menu virtual; otherwise it runs the base dispatch (0x18028ac80).
// Neither the global nor the virtual is modeled, so the body is conservative.
// Symbol: ?OnRButtonUp@CMFCCaptionBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonUp_CMFCCaptionBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- the retail body (RVA 0x224e0)
    // gates the context-menu path on the global flag at 0x1803be35c and the
    // vslot-0x4a0 virtual; neither is modeled.
}

//=============================================================================
// 4. Window message handlers
//=============================================================================

// OnCreate(CREATESTRUCTW*): runs the base CWnd::OnCreate (0x18028ac80) and
// returns -1 on failure; otherwise registers the tooltip helper (0x408) and
// adds four caption elements to it, then returns 0.  The tooltip-manager
// machinery is unmodeled; the success terminal (0) is returned.
// Symbol: ?OnCreate@CMFCCaptionBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCCaptionBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* /*lpCreateStruct*/)
{
    if (pThis == nullptr) return -1;
    // TODO(clean-room): transcribed partially -- retail runs the base
    // CWnd::OnCreate (0x18028ac80) first and registers the four caption
    // elements with the tooltip helper (0x408) through 0x180275060; none of
    // that is modeled, so the success terminal 0 is returned.
    return 0;
}

// OnDestroy(): releases the tooltip helper at 0x408 (0x1801824a0) and then
// runs the base CWnd::OnDestroy.  Transcribed from retail RVA 0x22570.
// Symbol: ?OnDestroy@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): retail first runs the tooltip-helper cleanup on
    // &this->0x408 (0x1801824a0); the helper object is not modeled.
    impl__OnDestroy_CWnd__IEAAXXZ(reinterpret_cast<CWnd*>(pThis));
}

// OnEraseBkgnd(CDC*): the retail body (RVA 0x3a60) always returns TRUE
// (background painting is handled by OnPaint / OnDrawBackground).  Fully
// transcribed.
// Symbol: ?OnEraseBkgnd@CMFCCaptionBar@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCCaptionBar__IEAAHPEAVCDC___Z(
    void* pThis, CDC* /*pDC*/)
{
    (void)pThis;
    return TRUE;
}

// OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*): adjusts the first client rect by
// the border height (0x684): top += height, bottom -= height.  Fully
// transcribed from retail RVA 0x203d0.
// Symbol: ?OnNcCalcSize@CMFCCaptionBar@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCCaptionBar__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(
    void* pThis, int /*bCalcValidRects*/, NCCALCSIZE_PARAMS* pParams)
{
    if (pThis == nullptr || pParams == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    pParams->rgrc[0].top += s->m_nBorderHeight;
    pParams->rgrc[0].bottom -= s->m_nBorderHeight;
}

// OnNcPaint(): paints the non-client border.  The retail body (RVA 0x20750)
// resolves the window rects, adjusts them by the border height (0x684) and
// runs the vslot-0x658 draw virtual; the draw path is not modeled.
// Symbol: ?OnNcPaint@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x20750)
    // computes the client/non-client rects and runs the vslot-0x658 draw
    // virtual; not modeled.
}

// OnNeedTipText(UINT, NMHDR*, LRESULT*): fills the tooltip text for the four
// caption elements.  The retail body (RVA 0x225a0) validates the NMHDR
// against the tooltip helper (0x408) and dispatches on pNMHDR->idFrom to the
// per-element tooltip strings; the tooltip-manager state is unmodeled, so the
// guard-terminal FALSE is returned.
// Symbol: ?OnNeedTipText@CMFCCaptionBar@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCCaptionBar__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    void* pThis, unsigned int /*id*/, NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    if (pThis == nullptr || pNMHDR == nullptr) return FALSE;
    CB* s = reinterpret_cast<CB*>(pThis);
    if (s->m_pToolTipWnd == nullptr) return FALSE;
    HWND hwndHelper =
        *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->m_pToolTipWnd) + 0x40);
    if (hwndHelper == nullptr) return FALSE;
    if (pNMHDR->hwndFrom != hwndHelper) return FALSE;
    // TODO(clean-room): transcribed partially -- retail dispatches on
    // pNMHDR->idFrom to the per-element tooltip strings (0x640/0x648,
    // 0x5c8/0x5d0, 0x5e0 + max-tip-width) through the tooltip-manager global
    // (0x1803b25e8); not modeled, so the guard-terminal FALSE is returned.
    return FALSE;
}

// OnPaint(): paints the caption bar.  The retail body (RVA 0x203f0) builds a
// CPaintDC, resolves the client rect, selects the caption font and dispatches
// the OnDrawBackground/OnDrawText/OnDrawImage/OnDrawButton virtuals; the draw
// path is not modeled.
// Symbol: ?OnPaint@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x203f0) builds
    // a CPaintDC and dispatches the element draw virtuals (vslot 0x650..0x670)
    // plus the vslot-0x60/0x70 font-selection virtuals; not modeled.
}

// OnSize(UINT, int, int): after the internal size hook (0x18000c2a0) and the
// vslot-0x430 layout virtual the whole client area is invalidated.
// Conservative -- the unmodeled hooks are skipped, the invalidate is kept.
// Symbol: ?OnSize@CMFCCaptionBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCCaptionBar__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    // TODO(clean-room): retail runs the internal size hook (0x18000c2a0) and
    // the vslot-0x430 layout virtual before invalidating.
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
    }
}

// OnSysColorChange(): the retail body (RVA 0x27d0) is an empty `ret`;
// transcribed as a no-op.
// Symbol: ?OnSysColorChange@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    (void)pThis;
}

// OnUpdateCmdUI(CFrameWnd*, BOOL): the retail body (RVA 0x27d0) is an empty
// `ret`; transcribed as a no-op.
// Symbol: ?OnUpdateCmdUI@CMFCCaptionBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCCaptionBar__MEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* /*pTarget*/, int /*bDisableIfNoHndler*/)
{
    (void)pThis;
}

// OnUpdateToolTips(WPARAM, LPARAM): relays the mouse-tracking flag (wParam
// bit 0x400) into the tooltip helper by registering the four caption
// elements.  The retail body (RVA 0x22810) drives the tooltip manager
// (0x180275060 x4); the guard checks are kept.
// Symbol: ?OnUpdateToolTips@CMFCCaptionBar@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnUpdateToolTips_CMFCCaptionBar__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    if ((wParam & 0x400) != 0) {
        CB* s = reinterpret_cast<CB*>(pThis);
        if (s->m_pToolTipWnd != nullptr) {
            HWND hwndHelper =
                *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->m_pToolTipWnd) + 0x40);
            if (hwndHelper != nullptr) {
                // TODO(clean-room): retail registers the four caption elements
                // with the tooltip helper (0x180275060, elements 1..4); not
                // modeled.
            }
        }
    }
    return 0;
}

// PreTranslateMessage(MSG*): relays keyboard (WM_KEYDOWN / WM_SYSKEYDOWN) and
// mouse (WM_MOUSEMOVE..WM_MBUTTONUP, excluding the double-click ids) messages
// to the tooltip helper via TTM_RELAYEVENT, then tail-calls the base
// CWnd::PreTranslateMessage.  Transcribed from retail RVA 0x22780.
// Symbol: ?PreTranslateMessage@CMFCCaptionBar@@MEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCCaptionBar__MEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    CB* s = reinterpret_cast<CB*>(pThis);
    const unsigned int msg = pMsg->message;
    const bool bRelay =
        (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN ||
         msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP ||
         msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP ||
         msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP);
    if (bRelay && s->m_pToolTipWnd != nullptr) {
        HWND hwndHelper =
            *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->m_pToolTipWnd) + 0x40);
        if (hwndHelper != nullptr) {
            ::SendMessageW(hwndHelper, 0x407 /* TTM_RELAYEVENT */, 0,
                           reinterpret_cast<LPARAM>(pMsg));
        }
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(
        reinterpret_cast<CWnd*>(pThis), pMsg);
}

// RemoveBitmap(): clears the image object at 0x418 (0x18016f690) and
// tail-calls the vslot-0x428 base dispatch.  The image object and the base
// dispatch are not modeled.
// Symbol: ?RemoveBitmap@CMFCCaptionBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveBitmap_CMFCCaptionBar__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail clears the image
    // object at 0x418 (0x18016f690) and tail-calls the vslot-0x428 base
    // dispatch; neither is modeled.
}

//=============================================================================
// 5. Element drawing (conservative -- draw through the visual manager)
//=============================================================================

// OnDrawBackground(CDC*, CRect): fills the caption background.  The retail
// body (RVA 0x208a0) inflates the rect by the border height (0x684), gates on
// the m_bDrawBackground flag (0x404) and runs the CMFCVisualManager draw
// virtual (vslot 0x1b0 of the visual manager singleton at 0x180009774); the
// visual manager is not modeled.
// Symbol: ?OnDrawBackground@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawBackground_CMFCCaptionBar__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail draws through the
    // visual-manager singleton (0x180009774, vslot 0x1b0) using the
    // m_bDrawBackground flag (0x404) and the border height (0x684); not
    // modeled.
}

// OnDrawBorder(CDC*, CRect): inflates the passed rect by (2, 2) and then
// draws the border through the visual manager.  The rect inflate is
// transcribed (retail RVA 0x20980); the draw call is not modeled.
// Symbol: ?OnDrawBorder@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCCaptionBar__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect rect)
{
    if (pThis == nullptr) return;
    ::InflateRect(reinterpret_cast<LPRECT>(&rect), 2, 2);
    // TODO(clean-room): retail then runs the visual-manager
    // OnDrawCaptionBarBorder virtual (singleton 0x180009774, vslot 0x1b8)
    // with the border color (0x680) and the m_bTransparent flag (0x400); the
    // visual manager is not modeled.
}

// OnDrawButton(CDC*, CRect, const CString&, BOOL): draws the caption button.
// The retail body (RVA 0x209f0) inflates the local rect by the margin
// (0x68c), selects the button font and runs the visual-manager
// OnDrawCaptionBarButton virtual; the draw path is not modeled.
// Symbol: ?OnDrawButton@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__OnDrawButton_CMFCCaptionBar__MEAAXPEAVCDC__VCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, const CString& /*strText*/,
    int /*bEnabled*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x209f0) runs
    // the visual-manager OnDrawCaptionBarButton virtual (singleton 0x180009774,
    // vslot 0x1c0) with the button flags (0x67c/0x678/0x66c/0x668/0x670); the
    // visual manager is not modeled.
}

// OnDrawImage(CDC*, CRect): draws the caption image.  The retail body
// (RVA 0x20f20) either draws the image object at 0x410 directly or through
// the internal image renderer (0x18016c060); the image state is unmodeled.
// Symbol: ?OnDrawImage@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawImage_CMFCCaptionBar__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x20f20) draws
    // the image object at 0x410 (DrawIconEx / 0x18016c060); the image object
    // is not modeled.
}

// OnDrawText(CDC*, CRect, const CString&): draws the caption text.  The
// retail body (RVA 0x20d70) selects the caption font and runs the
// CDC::DrawTextW virtual with the DT_* flags; the draw path is not modeled.
// Symbol: ?OnDrawText@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__OnDrawText_CMFCCaptionBar__MEAAXPEAVCDC__VCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, CDC* /*pDC*/, CRect /*rect*/, const CString& /*strText*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail (RVA 0x20d70) selects
    // the caption font (0x5d8) and runs the CDC::DrawTextW virtual (vslot
    // 0xe0) with DT_CALCRECT-style flags; the draw path is not modeled.
}

//---------------------------------------------------------------------------
// 1.1 Element setters / removers
//---------------------------------------------------------------------------

// RemoveButton(): resets the button-text CString (0x638) and tail-calls the
// vslot-0x428 base dispatch.  Transcribed from retail RVA 0x21270.
// Symbol: ?RemoveButton@CMFCCaptionBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveButton_CMFCCaptionBar__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    EmptyCStr(&s->m_strButtonText);
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch
    // (0x1802c7b30); not modeled.
}

// RemoveIcon(): clears the m_pImage slot (0x410) and tail-calls the
// vslot-0x428 base dispatch.  Transcribed from retail RVA 0x21300.
// Symbol: ?RemoveIcon@CMFCCaptionBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveIcon_CMFCCaptionBar__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_pImage = nullptr;
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch
    // (0x1802c7b30); not modeled.
}

// RemoveText(): resets the caption-text CString (0x5e0) and tail-calls the
// vslot-0x428 base dispatch.  Transcribed from retail RVA 0x216b0.
// Symbol: ?RemoveText@CMFCCaptionBar@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveText_CMFCCaptionBar__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    EmptyCStr(&s->m_strText);
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch
    // (0x1802c7b30); not modeled.
}

// SetBitmap(UINT nID, ULONG ulTransparent, int nIndex, BarElementAlignment
// nAlign): stores nAlign into the button-alignment slot (0x5b4), nIndex into
// the bitmap-index slot (0x5b0), caches ulTransparent at 0x4f0 and clears the
// m_pImage slot (0x410).  Transcribed from retail RVA 0x213e0; the image
// object at 0x418 (resource load 0x18016b6c0 / reload 0x180171320) is
// unmodeled.
// Symbol: ?SetBitmap@CMFCCaptionBar@@QEAAXIKHW4BarElementAlignment@1@@Z
extern "C" void MS_ABI impl__SetBitmap_CMFCCaptionBar__QEAAXIKHW4BarElementAlignment_1__Z(
    void* pThis, unsigned int /*nID*/, unsigned long ulTransparent,
    int nIndex, int nAlign)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_pImage = nullptr;   // 0x410 cleared
    if (s->m_nTransparent != static_cast<int>(ulTransparent)) {
        s->m_nTransparent = static_cast<int>(ulTransparent);  // 0x4f0
    }
    // TODO(clean-room): retail also clears the image object at 0x418
    // (0x18016f690), loads the resource id (low word of nID, 0x18016b6c0) and
    // reloads it (0x180171320); the image object is not modeled.
    s->m_nBitmapIndex = nIndex;  // 0x5b0
    s->m_nButtonAlign = nAlign;  // 0x5b4
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch.
}

// SetBitmap(HBITMAP hBitmap, ULONG ulTransparent, int nIndex,
// BarElementAlignment nAlign): stores nAlign into the button-alignment slot
// (0x5b4), nIndex into the bitmap-index slot (0x5b0), caches ulTransparent at
// 0x4f0, clears m_pImage (0x410) and, via GetObjectW, records the bitmap size
// into 0x480/0x484.  Transcribed from retail RVA 0x21320 (a null bitmap fails
// fast in retail -- 0x180227720 -- which is conservatively a return here);
// the image-object load at 0x418 (0x18016d880) is unmodeled.
// Symbol: ?SetBitmap@CMFCCaptionBar@@QEAAXPEAUHBITMAP__@@KHW4BarElementAlignment@1@@Z
extern "C" void MS_ABI impl__SetBitmap_CMFCCaptionBar__QEAAXPEAUHBITMAP____KHW4BarElementAlignment_1__Z(
    void* pThis, HBITMAP hBitmap, unsigned long ulTransparent,
    int nIndex, int nAlign)
{
    if (pThis == nullptr) return;
    if (hBitmap == nullptr) {
        // TODO(clean-room): retail fails fast (0x180227720); conservative
        // return without touching state.
        return;
    }
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_pImage = nullptr;   // 0x410 cleared
    BITMAP bm = {};
    if (::GetObjectW(hBitmap, sizeof(BITMAP), &bm) != 0) {
        s->m_sizeImageCX = bm.bmWidth;   // 0x480
        s->m_sizeImageCY = bm.bmHeight;  // 0x484
    }
    if (s->m_nTransparent != static_cast<int>(ulTransparent)) {
        s->m_nTransparent = static_cast<int>(ulTransparent);  // 0x4f0
    }
    // TODO(clean-room): retail also clears the image object at 0x418
    // (0x18016f690) and re-loads hBitmap through the image-object setter
    // (0x18016d880); the image object is not modeled.
    s->m_nBitmapIndex = nIndex;  // 0x5b0
    s->m_nButtonAlign = nAlign;  // 0x5b4
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch.
}

// SetButton(LPCTSTR lpszText, UINT nID, BarElementAlignment nAlign, BOOL
// bIsClick): stores the text into the button-text CString (0x638), nID into
// 0x650, nAlign into the text-alignment slot (0x654) and bIsClick into 0x67c.
// Transcribed from retail RVA 0x210b0 (a null text fails fast in retail --
// 0x180227720 -- which is conservatively a return here).
// Symbol: ?SetButton@CMFCCaptionBar@@QEAAXPEB_WIW4BarElementAlignment@1@H@Z
extern "C" void MS_ABI impl__SetButton_CMFCCaptionBar__QEAAXPEB_WIW4BarElementAlignment_1_H_Z(
    void* pThis, const wchar_t* lpszText, unsigned int nID, int nAlign,
    int bIsClick)
{
    if (pThis == nullptr) return;
    if (lpszText == nullptr) {
        // TODO(clean-room): retail fails fast (0x180227720); conservative
        // return without touching state.
        return;
    }
    CB* s = reinterpret_cast<CB*>(pThis);
    AssignCStr(&s->m_strButtonText, lpszText);  // 0x638
    s->m_nID = static_cast<int>(nID);           // 0x650
    s->m_nTextAlign = nAlign;                   // 0x654
    s->m_bIsClick = bIsClick;                   // 0x67c
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch.
}

// SetButtonPressed(BOOL bPressed): stores bPressed into the button-highlight
// slot (0x670) and, while a window exists, invalidates + repaints the button
// rect (0x658).  Fully transcribed from retail RVA 0x211c0.
// Symbol: ?SetButtonPressed@CMFCCaptionBar@@QEAAXH@Z
extern "C" void MS_ABI impl__SetButtonPressed_CMFCCaptionBar__QEAAXH_Z(
    void* pThis, int bPressed)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    s->m_bButtonHighlight = bPressed;  // 0x670
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd,
                         reinterpret_cast<const RECT*>(&s->m_rectButton), TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}

// SetButtonToolTip(LPCTSTR lpszTip, LPCTSTR lpszDesc): stores the tip and the
// description into the button-tooltip CStrings (0x640 / 0x648, null defaults
// to the empty string in retail) and then runs UpdateTooltips.  Transcribed
// from retail RVA 0x21200.
// Symbol: ?SetButtonToolTip@CMFCCaptionBar@@QEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetButtonToolTip_CMFCCaptionBar__QEAAXPEB_W0_Z(
    void* pThis, const wchar_t* lpszTip, const wchar_t* lpszDesc)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    AssignCStr(&s->m_strButtonTip, lpszTip);        // 0x640
    AssignCStr(&s->m_strButtonTipDesc, lpszDesc);   // 0x648
    impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(pThis);
}

// SetIcon(HICON hIcon, BarElementAlignment nAlign): clears the image object
// at 0x418, stores hIcon into the m_pImage slot (0x410) and nAlign into the
// button-alignment slot (0x5b4).  Transcribed from retail RVA 0x212a0.
// Symbol: ?SetIcon@CMFCCaptionBar@@QEAAXPEAUHICON__@@W4BarElementAlignment@1@@Z
extern "C" void MS_ABI impl__SetIcon_CMFCCaptionBar__QEAAXPEAUHICON____W4BarElementAlignment_1__Z(
    void* pThis, HICON hIcon, int nAlign)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    // TODO(clean-room): retail clears the image object at 0x418
    // (0x18016f690); not modeled.
    s->m_pImage = hIcon;         // 0x410
    s->m_nButtonAlign = nAlign;  // 0x5b4
    // TODO(clean-room): retail tail-calls the vslot-0x428 base dispatch.
}

// SetImageToolTip(LPCTSTR lpszTip, LPCTSTR lpszDesc): stores the tip and the
// description into the image-tooltip CStrings (0x5c8 / 0x5d0, null defaults
// to the empty string in retail) and then runs UpdateTooltips.  Transcribed
// from retail RVA 0x214a0.
// Symbol: ?SetImageToolTip@CMFCCaptionBar@@QEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetImageToolTip_CMFCCaptionBar__QEAAXPEB_W0_Z(
    void* pThis, const wchar_t* lpszTip, const wchar_t* lpszDesc)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    AssignCStr(&s->m_strImageTip, lpszTip);        // 0x5c8
    AssignCStr(&s->m_strImageTipDesc, lpszDesc);   // 0x5d0
    impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(pThis);
}

//---------------------------------------------------------------------------
// 1.2 Tooltips
//---------------------------------------------------------------------------

// UpdateTooltips(): relays the four caption elements (gripper, text, image,
// button) to the tooltip helper.  The retail body (RVA 0x22910) guards on the
// tooltip-helper pointer (0x408) and its m_hWnd at +0x40 and then runs the
// tooltip-manager relay 0x180275480 four times with the per-element rects
// (0x6a4 gripper; 0x624 text gated on the 0x634 flag; 0x5b8 image gated on
// the image-tip length at 0x5c8; 0x658 button gated on the button-tip length
// at 0x640).  The guards are transcribed; the relay is not modeled.
// Symbol: ?UpdateTooltips@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CB* s = reinterpret_cast<CB*>(pThis);
    if (s->m_pToolTipWnd == nullptr) return;
    HWND hwndHelper =
        *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->m_pToolTipWnd) + 0x40);
    if (hwndHelper == nullptr) return;
    // TODO(clean-room): transcribed partially -- retail relays the four
    // caption elements to the tooltip helper through 0x180275480 (elements
    // 1..4 with the rects listed above); the tooltip manager is not modeled.
}
