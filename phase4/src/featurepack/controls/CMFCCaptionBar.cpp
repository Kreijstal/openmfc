// CMFCCaptionBar — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfccaptionbar.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCCaptionBarSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <atomic>
#include <cstddef>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Construction / destruction support (CreateObject, ??0, ??1 below).
//
// Retail member names for the offsets the constructor (RVA 0x1fc50, mfc140u)
// writes, assigned by declaration order in the shipping afxcaptionbar.h
// (`class CMFCCaptionBar : public CPane`) and consistent with every store the
// retail constructor makes -- in particular m_Bitmap (a 0x198-byte
// CMFCToolBarImages) fills 0x418..0x5b0 exactly and m_arTextParts (a
// 0x28-byte CStringArray) fills 0x5e8..0x610 exactly:
//   0x3f8 m_clrBarText   0x3fc m_clrBarBackground   0x400 m_clrBarBorder
//   0x404 m_bIsMessageBarMode   0x408 m_pToolTip   0x410 m_hIcon
//   0x418 m_Bitmap (CMFCToolBarImages)   0x5b0 m_bStretchImage
//   0x5b4 m_iconAlignment   0x5b8 m_rectImage   0x5c8 m_strImageToolTip
//   0x5d0 m_strImageDescription   0x5d8 m_hFont   0x5e0 m_strText
//   0x5e8 m_arTextParts (CStringArray)   0x610 m_textAlignment
//   0x614 m_rectText   0x624 m_rectDrawText   0x634 m_bTextIsTruncated
//   0x638 m_strBtnText   0x640 m_strButtonToolTip   0x648 m_strButtonDescription
//   0x650 m_uiBtnID   0x654 m_btnAlignnment   0x658 m_rectButton
//   0x668 m_bIsBtnPressed   0x66c m_bIsBtnHighlighted   0x670 m_bIsBtnForcePressed
//   0x674 m_bTracked   0x678 m_bBtnEnabled   0x67c m_bBtnHasDropDownArrow
//   0x680 m_bFlatBorder   0x684 m_nBorderSize   0x688 m_nMargin
//   0x68c m_nHorzElementOffset   0x690 m_nDefaultHeight   0x694 m_nCurrentHeight
//   0x698 m_bIsCloseBtnPressed   0x69c m_bIsCloseBtnHighlighted
//   0x6a0 m_bCloseTracked   0x6a4 m_rectClose   (0x6b4..0x6b8 tail pad)
// Several CB (detail/CMFCCaptionBarSupport.h) field NAMES disagree with this
// list (the offsets they sit at are what the code below relies on, and the
// ones used are pinned by static_assert); the constructor/destructor therefore
// address members by these retail offsets rather than by CB name.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                  // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CPane__IEAA_XZ(void* pThis);                    // docking/Thunks.cpp
extern "C" void  MS_ABI impl___1CPane__UEAA_XZ(void* pThis);                    // docking/Thunks.cpp
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis);        // toolbar/CMFCToolBarImages.cpp
extern "C" void  MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis);        // toolbar/CMFCToolBarImages.cpp
extern "C" void* MS_ABI impl___0CStringArray__QEAA_XZ(CStringArray* pThis);     // core/collections/CStringArray.cpp
extern "C" void  MS_ABI impl___1CStringArray__UEAA_XZ(CStringArray* pThis);     // core/collections/CStringArray.cpp
extern "C" void* MS_ABI impl___0CMFCCaptionBar__QEAA_XZ(void* pThis);           // defined below

namespace {

constexpr std::size_t kCapObjectSize       = 0x6b8;  // CreateObject: operator new(0x6b8)
constexpr std::size_t kOffClrBarText       = 0x3f8;
constexpr std::size_t kOffClrBarBackground = 0x3fc;
constexpr std::size_t kOffClrBarBorder     = 0x400;
constexpr std::size_t kOffIsMessageBarMode = 0x404;
constexpr std::size_t kOffToolTip          = 0x408;
constexpr std::size_t kOffIcon             = 0x410;
constexpr std::size_t kOffBitmap           = 0x418;
constexpr std::size_t kOffStretchImage     = 0x5b0;
constexpr std::size_t kOffIconAlignment    = 0x5b4;
constexpr std::size_t kOffRectImage        = 0x5b8;
constexpr std::size_t kOffStrImageToolTip  = 0x5c8;
constexpr std::size_t kOffStrImageDesc     = 0x5d0;
constexpr std::size_t kOffFont             = 0x5d8;
constexpr std::size_t kOffStrText          = 0x5e0;
constexpr std::size_t kOffArTextParts      = 0x5e8;
constexpr std::size_t kOffTextAlignment    = 0x610;
constexpr std::size_t kOffRectText         = 0x614;
constexpr std::size_t kOffRectDrawText     = 0x624;
constexpr std::size_t kOffTextIsTruncated  = 0x634;
constexpr std::size_t kOffStrBtnText       = 0x638;
constexpr std::size_t kOffStrBtnToolTip    = 0x640;
constexpr std::size_t kOffStrBtnDesc       = 0x648;
constexpr std::size_t kOffBtnID            = 0x650;
constexpr std::size_t kOffBtnAlignment     = 0x654;
constexpr std::size_t kOffRectButton       = 0x658;
constexpr std::size_t kOffBtnPressed       = 0x668;   // qword store covers 0x66c too
constexpr std::size_t kOffBtnForcePressed  = 0x670;   // qword store covers 0x674 too
constexpr std::size_t kOffBtnEnabled       = 0x678;
constexpr std::size_t kOffBtnDropDownArrow = 0x67c;   // qword store covers 0x680 too
constexpr std::size_t kOffBorderSize       = 0x684;
constexpr std::size_t kOffMargin           = 0x688;
constexpr std::size_t kOffHorzElemOffset   = 0x68c;
constexpr std::size_t kOffDefaultHeight    = 0x690;
constexpr std::size_t kOffCurrentHeight    = 0x694;
constexpr std::size_t kOffCloseBtnPressed  = 0x698;   // qword store covers 0x69c too
constexpr std::size_t kOffCloseTracked     = 0x6a0;
constexpr std::size_t kOffRectClose        = 0x6a4;

// The embedded sub-objects built through OpenMFC thunks must fit the retail
// slots they occupy (the next retail member starts right after each).
static_assert(sizeof(CPane) == kOffClrBarText, "CMFCCaptionBar members start right after the 0x3f8-byte CPane base");
static_assert(sizeof(CMFCToolBarImages) == 0x198, "CMFCToolBarImages retail size (afxmfc.h harvested layout)");
static_assert(kOffBitmap + sizeof(CMFCToolBarImages) == kOffStretchImage, "m_Bitmap fills 0x418..0x5b0");
static_assert(sizeof(CString) == 8, "CString is one pointer to the character data");
// OpenMFC's CStringArray is only its 8-byte vptr (element storage lives in a
// side table keyed by `this`), so it fits inside the 0x28-byte retail slot;
// the rest of the slot is zeroed exactly as retail's inline ctor does.
static_assert(sizeof(CStringArray) <= kOffTextAlignment - kOffArTextParts, "m_arTextParts slot is 0x28 bytes");
static_assert(sizeof(CB) == kCapObjectSize, "CB mirror is the 0x6b8-byte retail object");
// Cross-checks against the CB fields other bodies in this file use.
static_assert(offsetof(CB, m_pToolTipWnd) == kOffToolTip, "CB m_pToolTipWnd @0x408");
static_assert(offsetof(CB, m_pImage) == kOffIcon, "CB m_pImage @0x410");
static_assert(offsetof(CB, m_strImageTip) == kOffStrImageToolTip, "CB m_strImageTip @0x5c8");
static_assert(offsetof(CB, m_strImageTipDesc) == kOffStrImageDesc, "CB m_strImageTipDesc @0x5d0");
static_assert(offsetof(CB, m_hFont) == kOffFont, "CB m_hFont @0x5d8");
static_assert(offsetof(CB, m_strText) == kOffStrText, "CB m_strText @0x5e0");
static_assert(offsetof(CB, m_strButtonText) == kOffStrBtnText, "CB m_strButtonText @0x638");
static_assert(offsetof(CB, m_strButtonTip) == kOffStrBtnToolTip, "CB m_strButtonTip @0x640");
static_assert(offsetof(CB, m_strButtonTipDesc) == kOffStrBtnDesc, "CB m_strButtonTipDesc @0x648");
static_assert(offsetof(CB, m_rectButton) == kOffRectButton, "CB m_rectButton @0x658");
static_assert(offsetof(CB, m_rectGripper) == kOffRectClose, "CB m_rectGripper @0x6a4");

template <typename T>
inline T& CapField(void* pThis, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(pThis) + off);
}
inline CString* CapStr(void* pThis, std::size_t off) {
    return reinterpret_cast<CString*>(static_cast<char*>(pThis) + off);
}
inline RECT* CapRect(void* pThis, std::size_t off) {
    return reinterpret_cast<RECT*>(static_cast<char*>(pThis) + off);
}

// The vptr the CPane constructor thunk installs, recorded by ??0 and
// re-stored by ??1 (same scheme as docking/CPaneDialog.cpp): OpenMFC has no
// CMFCCaptionBar vftable, and a client subclass's destructor leaves its own
// vftable in place, while the CPane destructor thunk destroys through the
// object's vptr.
std::atomic<void*> g_captionBarPaneVptr{nullptr};

} // namespace

// Symbol: ?SetText@CMFCCaptionBar@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@W4BarElementAlignment@1@@Z
extern "C" void MS_ABI impl__SetText_CMFCCaptionBar__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4BarElementAlignment_1__Z(
    CMFCCaptionBar* pThis, const CString* pText, int nAlignment) {
    if (!pThis) {
        return;
    }

    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    auto& state = g_captionBarText[pThis];
    state.text = pText ? (const wchar_t*)(*pText) : L"";
    state.alignment = nAlignment;
}
// Symbol: ?AdjustLayout@CMFCCaptionBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCCaptionBar__MEAAXXZ(CMFCCaptionBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    ++g_captionBarText[pThis].layoutVersion;
}
// Symbol: ?RecalcLayout@CMFCCaptionBar@@MEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCCaptionBar__MEAAXXZ(CMFCCaptionBar* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_captionBarTextMutex);
    ++g_captionBarText[pThis].layoutVersion;
}
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
// CreateObject(): CRuntimeClass factory.  Retail (RVA 0x1fc10, mfc140u),
// fully transcribed:
//     void* p = operator new(0x6b8);                 // ??2@YAPEAX_K@Z, RVA 0x27f0
//     return p ? CMFCCaptionBar::CMFCCaptionBar(p) : NULL;   // ctor RVA 0x1fc50
// OpenMFC's ??2 thunk is the allocator.  It returns NULL on failure (malloc)
// where retail's operator new throws, so the null branch is reachable here;
// retail's new-expression EH cleanup (free the block if the ctor throws) is
// not reproduced.
// Symbol: ?CreateObject@CMFCCaptionBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCCaptionBar__SAPEAVCObject__XZ(void)
{
    void* p = impl___2_YAPEAX_K_Z(kCapObjectSize);
    if (p == nullptr) return nullptr;
    return impl___0CMFCCaptionBar__QEAA_XZ(p);
}
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
// GetAlignment(BarElement): returns the stored alignment for ELEM_BUTTON (0,
// m_btnAlignnment@0x654), ELEM_TEXT (1, m_textAlignment@0x610) or ELEM_ICON
// (2, m_iconAlignment@0x5b4) -- afxcaptionbar.h enum order; the CB field
// names below are misleading -- every other id returns 0.  Fully
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
// OnEraseBkgnd(CDC*): always returns TRUE (background painting is done by
// OnPaint / OnDrawBackground).  This export has no entry of its own in the
// RVA symbol map; resolved through its ordinal and the mfc140u export table
// it points at RVA 0x3a60 (mfc140u), a two-instruction body shared by
// identical-code folding (the map names that address
// CMFCBaseAccessibleObject::accDoDefaultAction):
//     mov $0x1,%eax ; ret
// Fully transcribed; `this` and pDC are unused in retail too.
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
    // 0x5c8/0x5d0, 0x5e0 + max-tip-width) into a static CString at
    // 0x1803c1460 (mfc140u); 0x1803b25e8 (mfc140u), referenced at 0x1800225ec,
    // is the CString manager (vslot 3 GetNilString, +0x18) building a local
    // empty CString -- not a tooltip manager.  It also returns FALSE when the
    // global at 0x1803be288 is non-null.  When every guard passes retail
    // returns TRUE; that fill path is not modeled, so FALSE is returned.
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
// OnSysColorChange(): a no-op.  No entry of its own in the RVA symbol map;
// resolved through its ordinal and the mfc140u export table it points at RVA
// 0x27d0 (mfc140u), a folded body shared by 158 exports of the mfc140u
// export table (the map names that address CFrameWndEx::AddDockSite)
// consisting of the single instruction `ret $0x0`.  Fully transcribed.
// Symbol: ?OnSysColorChange@CMFCCaptionBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCCaptionBar__IEAAXXZ(
    void* pThis)
{
    (void)pThis;
}
// OnUpdateCmdUI(CFrameWnd*, BOOL): a no-op.  Resolved through its ordinal
// and the mfc140u export table to the same folded RVA 0x27d0 (mfc140u) as
// OnSysColorChange above -- the single instruction `ret $0x0`.  Fully
// transcribed.
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
// OnDrawBorder(CDC*, CRect): inflates the passed rect by (2, 0) --
// ::InflateRect(&rect, 2, 0), IAT slot 0x1802c72e8 resolved with iatu.py --
// and then draws the border through the visual manager.  The rect inflate is
// transcribed (retail RVA 0x20980, mfc140u); the draw call is not modeled.
// Symbol: ?OnDrawBorder@CMFCCaptionBar@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCCaptionBar__MEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* /*pDC*/, CRect rect)
{
    if (pThis == nullptr) return;
    ::InflateRect(reinterpret_cast<LPRECT>(&rect), 2, 0);
    // TODO(clean-room): retail then runs the visual-manager
    // OnDrawCaptionBarBorder virtual (singleton 0x180009774, vslot 0x1b8)
    // with m_clrBarBorder (0x400) and m_bFlatBorder (0x680); the
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
// nAlign): stores nAlign into m_iconAlignment (0x5b4), the third argument
// (bStretch in afxcaptionbar.h) into m_bStretchImage (0x5b0; CB calls it
// m_nBitmapIndex), caches ulTransparent at 0x4f0 and clears the
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
// BarElementAlignment nAlign): stores nAlign into m_iconAlignment (0x5b4),
// the third argument (bStretch in afxcaptionbar.h) into m_bStretchImage
// (0x5b0; CB calls it m_nBitmapIndex), caches ulTransparent at
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
// 0x650, nAlign into m_btnAlignnment (0x654) and the fourth argument
// (bHasDropDownArrow in afxcaptionbar.h) into 0x67c.
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
    AssignCStr_Cmfccaptionbar(&s->m_strButtonText, lpszText);  // 0x638
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
    AssignCStr_Cmfccaptionbar(&s->m_strButtonTip, lpszTip);        // 0x640
    AssignCStr_Cmfccaptionbar(&s->m_strButtonTipDesc, lpszDesc);   // 0x648
    impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(pThis);
}
// SetIcon(HICON hIcon, BarElementAlignment nAlign): clears the image object
// at 0x418, stores hIcon into the m_pImage slot (0x410) and nAlign into the
// m_iconAlignment slot (0x5b4).  Transcribed from retail RVA 0x212a0.
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
    AssignCStr_Cmfccaptionbar(&s->m_strImageTip, lpszTip);        // 0x5c8
    AssignCStr_Cmfccaptionbar(&s->m_strImageTipDesc, lpszDesc);   // 0x5d0
    impl__UpdateTooltips_CMFCCaptionBar__IEAAXXZ(pThis);
}
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
// CMFCCaptionBar::CMFCCaptionBar() -- retail entry RVA 0x1fc50 (mfc140u),
// transcribed store for store (names per the table at the top of the file):
//     CPane::CPane();                                    // call RVA 0x9f2f0
//     vfptr = &CMFCCaptionBar::`vftable';                // 0x1802df728 -- NOT reproduced
//     m_Bitmap.CMFCToolBarImages();                      // call RVA 0x16b0f0, on +0x418
//     m_rectImage = CRect();                             // two zero qwords at +0x5b8
//     m_strImageToolTip, m_strImageDescription, m_strText = CString();
//         -- each: the string manager's vslot 3 (GetNilString) + 0x18
//     m_arTextParts.CStringArray();  // inline: CStringArray vftable at +0x5e8,
//                                    // zero +0x5f0 / +0x608 / +0x600 / +0x5f8
//     m_rectText = m_rectDrawText = CRect();             // +0x614, +0x624
//     m_strBtnText, m_strButtonToolTip, m_strButtonDescription = CString();
//     m_rectButton = m_rectClose = CRect();              // +0x658, +0x6a4
//     m_pToolTip = NULL;
//     m_clrBarText = m_clrBarBackground = m_clrBarBorder = (COLORREF)-1;
//     m_nBorderSize = m_nMargin = m_nHorzElementOffset = 4;
//     m_hIcon = NULL;  m_hFont = NULL;
//     m_nDefaultHeight = -1;  m_nCurrentHeight = 0;
//     m_btnAlignnment = m_iconAlignment = m_textAlignment = ALIGN_LEFT (1);
//     m_bStretchImage = FALSE;
//     *(QWORD*)&m_bBtnHasDropDownArrow = 1;   // m_bBtnHasDropDownArrow = TRUE, m_bFlatBorder = FALSE
//     m_uiBtnID = 0;
//     *(QWORD*)&m_bIsBtnPressed = 0;          // m_bIsBtnPressed, m_bIsBtnHighlighted
//     *(QWORD*)&m_bIsBtnForcePressed = 0;     // m_bIsBtnForcePressed, m_bTracked
//     m_bBtnEnabled = TRUE;
//     ::SetRectEmpty(&m_rectImage / &m_rectText / &m_rectDrawText / &m_rectButton);
//     m_bTextIsTruncated = FALSE;  m_bIsMessageBarMode = FALSE;
//     *(QWORD*)&m_bIsCloseBtnPressed = 0;     // + m_bIsCloseBtnHighlighted
//     m_bCloseTracked = FALSE;
//     ::SetRectEmpty(&m_rectClose);
//     return this;
// (The SetRectEmpty import slot 0x1802c7348 was resolved with iatu.py.)
// DEVIATIONS, forced by what OpenMFC has:
//  * No CMFCCaptionBar vftable exists in OpenMFC, so the object keeps the
//    C++ CPane vptr the CPane ctor thunk installed; it is recorded for the
//    destructor (the recording scheme of docking/CPaneDialog.cpp;
//    controls/CMFCReBar.cpp likewise leaves the CPane vptr in place but has
//    no destructor that re-stores it).
//  * m_arTextParts is built with the OpenMFC CStringArray ctor thunk, whose
//    vptr is the OpenMFC CStringArray one and whose element storage is a side
//    table keyed by the address; the retail m_pData/m_nSize/m_nMaxSize/
//    m_nGrowBy words (+0x5f0..+0x60f) are still zeroed as retail does.
//  * The redundant zero stores of the inline CRect() ctors are folded into the
//    ::SetRectEmpty calls retail makes on the same five rects right after.
//  * A NULL `this` returns NULL; retail never checks it.
// Symbol: ??0CMFCCaptionBar@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCaptionBar__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CPane__IEAA_XZ(pThis);
    void* vptr = nullptr;
    std::memcpy(&vptr, pThis, sizeof vptr);
    g_captionBarPaneVptr.store(vptr, std::memory_order_relaxed);

    impl___0CMFCToolBarImages__QEAA_XZ(static_cast<char*>(pThis) + kOffBitmap);
    new (CapStr(pThis, kOffStrImageToolTip)) CString();
    new (CapStr(pThis, kOffStrImageDesc)) CString();
    new (CapStr(pThis, kOffStrText)) CString();
    std::memset(static_cast<char*>(pThis) + kOffArTextParts, 0,
                kOffTextAlignment - kOffArTextParts);
    impl___0CStringArray__QEAA_XZ(
        reinterpret_cast<CStringArray*>(static_cast<char*>(pThis) + kOffArTextParts));
    new (CapStr(pThis, kOffStrBtnText)) CString();
    new (CapStr(pThis, kOffStrBtnToolTip)) CString();
    new (CapStr(pThis, kOffStrBtnDesc)) CString();

    CapField<void*>(pThis, kOffToolTip) = nullptr;
    CapField<int>(pThis, kOffClrBarText) = -1;
    CapField<int>(pThis, kOffClrBarBackground) = -1;
    CapField<int>(pThis, kOffClrBarBorder) = -1;
    CapField<int>(pThis, kOffBorderSize) = 4;
    CapField<int>(pThis, kOffMargin) = 4;
    CapField<int>(pThis, kOffHorzElemOffset) = 4;
    CapField<void*>(pThis, kOffIcon) = nullptr;
    CapField<void*>(pThis, kOffFont) = nullptr;
    CapField<int>(pThis, kOffDefaultHeight) = -1;
    CapField<int>(pThis, kOffCurrentHeight) = 0;
    CapField<int>(pThis, kOffBtnAlignment) = 1;    // ALIGN_LEFT
    CapField<int>(pThis, kOffIconAlignment) = 1;   // ALIGN_LEFT
    CapField<int>(pThis, kOffTextAlignment) = 1;   // ALIGN_LEFT
    CapField<int>(pThis, kOffStretchImage) = 0;
    CapField<unsigned long long>(pThis, kOffBtnDropDownArrow) = 1;  // +0x67c = 1, +0x680 = 0
    CapField<int>(pThis, kOffBtnID) = 0;
    CapField<unsigned long long>(pThis, kOffBtnPressed) = 0;
    CapField<unsigned long long>(pThis, kOffBtnForcePressed) = 0;
    CapField<int>(pThis, kOffBtnEnabled) = 1;
    ::SetRectEmpty(CapRect(pThis, kOffRectImage));
    ::SetRectEmpty(CapRect(pThis, kOffRectText));
    ::SetRectEmpty(CapRect(pThis, kOffRectDrawText));
    ::SetRectEmpty(CapRect(pThis, kOffRectButton));
    CapField<int>(pThis, kOffTextIsTruncated) = 0;
    CapField<int>(pThis, kOffIsMessageBarMode) = 0;
    CapField<unsigned long long>(pThis, kOffCloseBtnPressed) = 0;
    CapField<int>(pThis, kOffCloseTracked) = 0;
    ::SetRectEmpty(CapRect(pThis, kOffRectClose));
    return pThis;
}

// CMFCCaptionBar::~CMFCCaptionBar() -- retail entry RVA 0x1ff10 (mfc140u),
// fully transcribed (members destroyed in reverse declaration order):
//     vfptr = &CMFCCaptionBar::`vftable';                // 0x1802df728
//     m_strButtonDescription.~CString();                  // +0x648 \  each an inline
//     m_strButtonToolTip.~CString();                      // +0x640  | CStringData release:
//     m_strBtnText.~CString();                            // +0x638  | lock xadd -1 on
//     m_arTextParts.~CStringArray();                      // call RVA 0x1d4db0, on +0x5e8
//     m_strText.~CString();                               // +0x5e0  | nRefs, and at <= 0
//     m_strImageDescription.~CString();                   // +0x5d0  | the manager's
//     m_strImageToolTip.~CString();                       // +0x5c8 /  vslot 1 (Free)
//     m_Bitmap.~CMFCToolBarImages();                      // call RVA 0x16b550, on +0x418
//     CPane::~CPane();                                    // tail jump RVA 0x9f490
// DEVIATION, forced: the vptr re-stored is the OpenMFC CPane one the
// constructor recorded (no CMFCCaptionBar vftable exists in OpenMFC).  The
// re-store matters for the reason retail does it: a client subclass's own
// destructor leaves its vftable in place, and the CPane dtor thunk destroys
// through the object's vptr.  A NULL `this` returns quietly; retail never
// checks it.
// Symbol: ??1CMFCCaptionBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCaptionBar__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    void* vptr = g_captionBarPaneVptr.load(std::memory_order_relaxed);
    if (vptr != nullptr) std::memcpy(pThis, &vptr, sizeof vptr);
    CapStr(pThis, kOffStrBtnDesc)->~CString();
    CapStr(pThis, kOffStrBtnToolTip)->~CString();
    CapStr(pThis, kOffStrBtnText)->~CString();
    impl___1CStringArray__UEAA_XZ(
        reinterpret_cast<CStringArray*>(static_cast<char*>(pThis) + kOffArTextParts));
    CapStr(pThis, kOffStrText)->~CString();
    CapStr(pThis, kOffStrImageDesc)->~CString();
    CapStr(pThis, kOffStrImageToolTip)->~CString();
    impl___1CMFCToolBarImages__UEAA_XZ(static_cast<char*>(pThis) + kOffBitmap);
    impl___1CPane__UEAA_XZ(pThis);
}
