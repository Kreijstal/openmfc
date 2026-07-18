// CMFCOutlookBarScrollButton: the little up/down scroll buttons that appear on
// an Outlook-style navigation bar. It derives from CMFCButton and overrides two
// protected drawing virtuals: OnFillBackground and OnDrawBorder. Only those two
// virtuals are exported (no ctor/dtor of its own is exported), so this file
// implements exactly those two thunks.
//
// The class is polymorphic (vfptr at offset 0), size(2848). We never touch any
// CMFCOutlookBarScrollButton member here -- both draw methods operate purely on
// the passed CDC and CRect -- so a full 2848-byte mirror is unnecessary; we only
// need to reach through CDC to its device-context handle.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful mirror of the head of CDC: {vfptr} at +0, m_hDC at +8
// (harvested via cl.exe /d1reportSingleClassLayout; see repo ABI notes).
struct CDC_head {
    void* vfptr;   // +0
    HDC   m_hDC;   // +8  output device context
    HDC   m_hAttribDC; // +16 attribute device context
};
static_assert(offsetof(CDC_head, m_hDC) == 8, "CDC::m_hDC offset");
static_assert(offsetof(CDC_head, m_hAttribDC) == 16, "CDC::m_hAttribDC offset");

static inline HDC DcHandle(void* pDC) {
    return pDC ? static_cast<CDC_head*>(pDC)->m_hDC : nullptr;
}

// UINT uiState bits used by the CMFCButton drawing pipeline for border state.
// (These mirror the observable "pressed"/"highlighted" distinctions.)
static const UINT SB_STATE_PRESSED     = 0x0001;
static const UINT SB_STATE_HIGHLIGHTED = 0x0002;

// Symbol: ?OnFillBackground@CMFCOutlookBarScrollButton@@EEAAXPEAVCDC@@AEBVCRect@@@Z
// virtual void OnFillBackground(CDC* pDC, const CRect& rectClient);
// Fills the scroll-button interior. The framework's default fill for an Outlook
// scroll button paints the button face colour across the client rectangle.
extern "C" void MS_ABI
impl__OnFillBackground_CMFCOutlookBarScrollButton__EEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    (void)pThis;
    HDC hdc = DcHandle(pDC);
    if (hdc == nullptr || rectClient == nullptr) {
        return;
    }
    // Paint the client area with the standard 3D button-face colour.
    HBRUSH hbr = ::GetSysColorBrush(COLOR_BTNFACE);
    if (hbr != nullptr) {
        ::FillRect(hdc, rectClient, hbr);
    }
}

// Symbol: ?OnDrawBorder@CMFCOutlookBarScrollButton@@EEAAXPEAVCDC@@AEAVCRect@@I@Z
// virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
// Draws the scroll button's raised (or, when pressed, sunken) border and then
// deflates rectClient by the consumed border so the caller draws content inside.
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCOutlookBarScrollButton__EEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, UINT uiState)
{
    (void)pThis;
    HDC hdc = DcHandle(pDC);
    if (hdc == nullptr || rectClient == nullptr) {
        return;
    }
    // A pressed button gets a sunken edge; otherwise a raised edge. DrawEdge with
    // BF_ADJUST shrinks rectClient in place by the space the border consumed.
    UINT edge = (uiState & SB_STATE_PRESSED) ? BDR_SUNKENOUTER : BDR_RAISEDINNER;
    ::DrawEdge(hdc, rectClient, edge, BF_RECT | BF_ADJUST);
}
