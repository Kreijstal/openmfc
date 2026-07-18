// OpenMFC: real implementation of CMFCDesktopAlertWndButton exported virtual overrides.
//
// CMFCDesktopAlertWndButton (afxdesktopalertwnd.h) derives from CMFCButton and is used
// exclusively inside a CMFCDesktopAlertWnd (the "toast" popup) as its caption/close
// button. Two BOOL flags select which glyph the button paints:
//     m_bIsCaptionButton  -> the caption drop/menu button (down arrow)
//     m_bIsCloseButton    -> the close button (an "X")
//
// The class overrides three protected CMFCButton drawing virtuals:
//
//   OnFillBackground  -- deliberately paints NOTHING. The desktop-alert window itself
//                        paints its caption gradient behind the button, so the button
//                        must stay transparent; the override exists purely to suppress
//                        CMFCButton's own face fill. (Same suppression pattern used by
//                        CMFCTabButton in this codebase.)
//
//   OnDrawBorder      -- deliberately paints NOTHING in the resting state. The alert
//                        buttons are borderless chrome on the caption bar; the override
//                        suppresses CMFCButton's default border draw.
//
//   OnDraw            -- paints the actual glyph (close "X" or caption down-arrow) using
//                        the button-text system colour, centred in the supplied rect.
//                        Real mfc140u renders this glyph through CMenuImages; that private
//                        bitmap sheet is unavailable here, so the glyph is reproduced with
//                        equivalent GDI primitives -- same observable shape/placement.
//
// Only these three virtuals are exported (no ctor/dtor), so -- exactly as with
// CMFCTabButton -- no vptr installation or vtable authoring is required here; the layout
// struct exists only to anchor the static_asserts and to read the two glyph flags.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view. Harvested size(2856): CObject vfptr@0, CCmdTarget m_dwRef@8.
// The CMFCButton base occupies the first 2848 bytes (== full CMFCTabButton size); this
// class appends the two glyph flags in declaration order:
//     m_bIsCaptionButton @ 2848, m_bIsCloseButton @ 2852.
struct S {
    void*         vfptr;             // 0:    CObject vtable pointer
    unsigned long m_dwRef;           // 8:    CCmdTarget reference count
    char          _base[2848 - 12];  //       remainder of the CMFCButton base
    int           m_bIsCaptionButton;// 2848: BOOL
    int           m_bIsCloseButton;  // 2852: BOOL
};

static_assert(sizeof(S) == 2856, "CMFCDesktopAlertWndButton size must match harvested layout");
static_assert(offsetof(S, vfptr) == 0, "vfptr at offset 0");
static_assert(offsetof(S, m_bIsCaptionButton) == 2848, "m_bIsCaptionButton at offset 2848");
static_assert(offsetof(S, m_bIsCloseButton) == 2852, "m_bIsCloseButton at offset 2852");

// CDC::m_hDC lives at offset 8 in the CDC object.
inline HDC hdc_of(void* pDC) {
    if (pDC == nullptr) return nullptr;
    return *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
}

} // namespace

// CMFCDesktopAlertWndButton keeps its background transparent: the owning alert window
// paints the caption gradient behind the button, so this override intentionally draws
// nothing (it exists only to suppress the CMFCButton face fill).
// Symbol: ?OnFillBackground@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI
impl__OnFillBackground_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* /*pThis*/, void* /*pDC*/, const RECT* /*rectClient*/)
{
    // Faithful empty override: the alert window owns the caption background.
}

// CMFCDesktopAlertWndButton is borderless chrome on the alert caption bar: this override
// suppresses the CMFCButton border draw and intentionally paints nothing in the resting
// state.
// Symbol: ?OnDrawBorder@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* /*pThis*/, void* /*pDC*/, RECT* /*rectClient*/, unsigned int /*uiState*/)
{
    // Faithful empty override: no border is drawn by the alert caption/close button.
}

// CMFCDesktopAlertWndButton::OnDraw paints the button glyph centred inside rect using the
// button-text system colour: an "X" for the close button, a down-arrow for the caption
// (menu) button. Real mfc140u draws this via CMenuImages; the private glyph sheet is not
// available here, so equivalent GDI primitives reproduce the same observable shape.
// Symbol: ?OnDraw@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDraw_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int /*uiState*/)
{
    if (pThis == nullptr || rect == nullptr) return;
    HDC hdc = hdc_of(pDC);
    if (hdc == nullptr) return;

    const S* p = reinterpret_cast<const S*>(pThis);

    const LONG w  = rect->right - rect->left;
    const LONG h  = rect->bottom - rect->top;
    if (w <= 0 || h <= 0) return;

    const LONG cx = rect->left + w / 2;
    const LONG cy = rect->top  + h / 2;
    LONG ext = ((w < h) ? w : h) / 4;   // half-extent of the glyph box
    if (ext < 2) ext = 2;

    const COLORREF clr = GetSysColor(COLOR_BTNTEXT);

    if (p->m_bIsCloseButton) {
        // Draw an "X": two diagonals across the centred glyph box.
        HPEN   pen = CreatePen(PS_SOLID, 1, clr);
        HGDIOBJ old = SelectObject(hdc, pen);

        MoveToEx(hdc, cx - ext, cy - ext, nullptr);
        LineTo  (hdc, cx + ext + 1, cy + ext + 1);
        MoveToEx(hdc, cx + ext, cy - ext, nullptr);
        LineTo  (hdc, cx - ext - 1, cy + ext + 1);

        SelectObject(hdc, old);
        DeleteObject(pen);
    } else if (p->m_bIsCaptionButton) {
        // Draw a filled down-pointing arrow (the caption/menu button glyph).
        HBRUSH  br     = CreateSolidBrush(clr);
        HGDIOBJ oldBr  = SelectObject(hdc, br);
        HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(NULL_PEN));

        POINT pts[3] = {
            { cx - ext, cy - ext / 2 },
            { cx + ext, cy - ext / 2 },
            { cx,       cy + ext      }
        };
        Polygon(hdc, pts, 3);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBr);
        DeleteObject(br);
    }
    // Neither flag set: nothing to draw (matches an unconfigured button).
}
