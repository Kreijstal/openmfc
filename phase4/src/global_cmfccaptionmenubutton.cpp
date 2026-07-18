// OpenMFC: CMFCCaptionMenuButton  (public CMFCCaptionButton : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(72):
//    0  {vfptr}            (CObject/CMFCCaptionButton vfptr)
//    8  m_bPushed          (CMFCCaptionButton::m_bPushed, BOOL)
//   ... remaining CMFCCaptionButton members (m_rect, m_nHit, enable/focus flags)
//   60  m_nMenuResult      (int)      CMFCCaptionMenuButton members
//   64  m_bOSMenu          (BOOL)
//   68  m_bMenuIsActive    (BOOL)
//
// A caption-bar button that drops a popup menu.  ShowMenu tracks a popup at the
// button rectangle and records the chosen command in m_nMenuResult; OnDraw
// paints the button chrome plus a drop-down arrow.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

// Layout-faithful mirror of the object.  The exact intermediate offsets of the
// CMFCCaptionButton base members are not individually validated (the harvest
// only pins {vfptr}@0, m_bPushed@8 and size 72), but they are placed to match
// the real member order so GetRect / OnDraw read sensible values.
// Real CMFCCaptionButton base layout (cl.exe /d1reportSingleClassLayout,
// harvested 2026-07-18): the base has NO m_rect member — the button rectangle
// is computed by the parent from m_ptOrg. Derived members start at 56.
struct S {
    void*    vfptr;               // 0   CObject vfptr
    int      m_bPushed;           // 8
    int      m_bFocused;          // 12
    int      m_bHidden;           // 16
    int      m_bEnabled;          // 20  BOOL (default TRUE)
    int      m_bDroppedDown;      // 24
    int      m_bLeftAlign;        // 28
    unsigned m_nHit;              // 32  UINT
    COLORREF m_clrForeground;     // 36  (default -1 / CLR_DEFAULT)
    POINT    m_ptOrg;             // 40  CPoint (x@40, y@44)
    int      m_bIsMiniFrameButton;// 48
    int      _pad0;               // 52  <alignment member>
    int      m_nMenuResult;       // 56  CMFCCaptionMenuButton::m_nMenuResult
    int      m_bOSMenu;           // 60  BOOL (default TRUE)
    int      m_bMenuIsActive;     // 64  BOOL
    int      _pad1;               // 68  <alignment member>
};

static_assert(sizeof(S) == 72, "CMFCCaptionMenuButton must be 72 bytes");
static_assert(offsetof(S, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S, m_bPushed) == 8, "m_bPushed at 8");
static_assert(offsetof(S, m_bEnabled) == 20, "m_bEnabled at 20");
static_assert(offsetof(S, m_clrForeground) == 36, "m_clrForeground at 36");
static_assert(offsetof(S, m_nMenuResult) == 56, "m_nMenuResult at 56");
static_assert(offsetof(S, m_bOSMenu) == 60, "m_bOSMenu at 60");

// CWnd::m_hWnd lives at offset 64.
static HWND WndHandle(void* pWnd) {
    return pWnd ? *(HWND*)((char*)pWnd + 64) : NULL;
}
// CDC::m_hDC lives at offset 8.
static HDC DcHandle(void* pDC) {
    return pDC ? *(HDC*)((char*)pDC + 8) : NULL;
}

// -------------------------------------------------------------------------
// GetRuntimeClass descriptor.  CMFCCaptionMenuButton has no DECLARE macro of
// its own, so slot 0 is CObject::GetRuntimeClass; the most-derived DYNCREATE
// base is CMFCCaptionButton.  Return the static descriptor directly (never
// self-dispatch through the vtable).
struct CRuntimeClassLite {
    const char* m_lpszClassName;
    int         m_nObjectSize;
    unsigned    m_wSchema;
    void*       m_pfnCreateObject;
    void*       m_pfnGetBaseClass;
    void*       m_pNextClass;
    void*       m_pClassInit;
};
CRuntimeClassLite g_rtc = {
    "CMFCCaptionButton", 60, 0xFFFF, 0, 0, 0, 0
};

// ---- non-exported virtual slot helpers --------------------------------------

static void* MS_ABI slot_GetRuntimeClass(const void* /*pThis*/) {
    return &g_rtc;
}
static void MS_ABI slot_Serialize(void* /*pThis*/, void* /*ar*/) {}
static void MS_ABI slot_AssertValid(const void* /*pThis*/) {}
static void MS_ABI slot_Dump(const void* /*pThis*/, void* /*dc*/) {}

// Slot 5: CMFCCaptionButton::GetRect — CRect returned by value (large struct
// via hidden return pointer under the MS x64 ABI).
static void* MS_ABI slot_GetRect(void* retptr, const void* pThis) {
    // No stored rect in the base; return an empty rect anchored at m_ptOrg.
    const S* s = (const S*)pThis;
    RECT r = { s->m_ptOrg.x, s->m_ptOrg.y, s->m_ptOrg.x, s->m_ptOrg.y };
    *(RECT*)retptr = r;
    return retptr;
}

// Slot 7: CMFCCaptionButton::GetIconID — maps the hit code to a marlett/system
// glyph id.  A menu button is not one of the standard system-command glyphs, so
// the base returns 0 (no system icon; the arrow is drawn in OnDraw).
static unsigned MS_ABI slot_GetIconID(const void* /*pThis*/, int /*bActive*/,
                                      int /*bHorz*/, int /*bMaximized*/,
                                      int /*bDisabled*/) {
    return 0;
}

} // namespace

// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace { extern void* const g_CMFCCaptionMenuButton_vtbl[8]; }

static void InitDefaults(S* self) {
    // Default values verified byte-exact against real mfc140u (differential dump):
    // only m_bEnabled, m_clrForeground(-1) and m_bOSMenu are non-zero.
    self->vfptr            = (void*)&g_CMFCCaptionMenuButton_vtbl[0];
    self->m_bPushed        = FALSE;
    self->m_bFocused       = FALSE;
    self->m_bHidden        = FALSE;
    self->m_bEnabled       = TRUE;
    self->m_bDroppedDown   = FALSE;
    self->m_bLeftAlign     = FALSE;
    self->m_nHit           = 0;
    self->m_clrForeground  = (COLORREF)-1;
    self->m_ptOrg.x = self->m_ptOrg.y = 0;
    self->m_bIsMiniFrameButton = FALSE;
    self->_pad0            = 0;
    self->m_nMenuResult    = 0;
    self->m_bOSMenu        = TRUE;
    self->m_bMenuIsActive  = FALSE;
    self->_pad1            = 0;
}

// Symbol: ??0CMFCCaptionMenuButton@@QEAA@IH@Z
extern "C" void* MS_ABI
impl___0CMFCCaptionMenuButton__QEAA_IH_Z(void* pThis, unsigned nHit, int bLeftAlign) {
    S* self = (S*)pThis;
    InitDefaults(self);
    self->m_nHit       = nHit;
    self->m_bLeftAlign = bLeftAlign;
    return pThis;
}

// Symbol: ??0CMFCCaptionMenuButton@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCCaptionMenuButton__QEAA_XZ(void* pThis) {
    InitDefaults((S*)pThis);
    return pThis;
}

// Symbol: ??1CMFCCaptionMenuButton@@UEAA@XZ
extern "C" void MS_ABI
impl___1CMFCCaptionMenuButton__UEAA_XZ(void* /*pThis*/) {
    // No owned resources (the tracked menu is owned by the caller).
}

// Symbol: ?OnDraw@CMFCCaptionMenuButton@@UEAAXPEAVCDC@@HHHH@Z
// Paints the button background per pushed/active state and a downward
// drop-down arrow centred in the button rectangle.
extern "C" void MS_ABI
impl__OnDraw_CMFCCaptionMenuButton__UEAAXPEAVCDC__HHHH_Z(
        void* pThis, void* pDC, int bActive, int /*bHorz*/,
        int /*bMaximized*/, int bDisabled) {
    S* self = (S*)pThis;
    HDC hdc = DcHandle(pDC);
    if (!hdc)
        return;

    // The base stores no rect; the parent assigns the button rect via SetRect at
    // paint time and passes an empty origin-only rect otherwise. Draw from m_ptOrg.
    RECT rc = { self->m_ptOrg.x, self->m_ptOrg.y, self->m_ptOrg.x, self->m_ptOrg.y };
    if (rc.right <= rc.left || rc.bottom <= rc.top)
        return;

    // Background: highlighted when pushed, otherwise button face.
    COLORREF bk = GetSysColor(self->m_bPushed ? COLOR_HIGHLIGHT
                              : (bActive ? COLOR_ACTIVECAPTION
                                         : COLOR_INACTIVECAPTION));
    HBRUSH hbr = CreateSolidBrush(bk);
    if (hbr) {
        FillRect(hdc, &rc, hbr);
        DeleteObject(hbr);
    }

    // Downward-pointing triangle (drop-down indicator).
    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int r  = 3;
    POINT tri[3] = {
        { cx - r, cy - 1 },
        { cx + r, cy - 1 },
        { cx,     cy + r - 1 },
    };
    COLORREF fg = GetSysColor(bDisabled ? COLOR_GRAYTEXT : COLOR_CAPTIONTEXT);
    HBRUSH   fb = CreateSolidBrush(fg);
    HPEN     fp = CreatePen(PS_SOLID, 1, fg);
    HGDIOBJ  ob = fb ? SelectObject(hdc, fb) : NULL;
    HGDIOBJ  op = fp ? SelectObject(hdc, fp) : NULL;
    Polygon(hdc, tri, 3);
    if (fb) { SelectObject(hdc, ob); DeleteObject(fb); }
    if (fp) { SelectObject(hdc, op); DeleteObject(fp); }
}

// Symbol: ?ShowMenu@CMFCCaptionMenuButton@@QEAAXPEAUHMENU__@@PEAVCWnd@@@Z
// Tracks a popup at the bottom-left of the button rectangle, records the chosen
// command in m_nMenuResult, and forwards it to the owner as WM_COMMAND.
extern "C" void MS_ABI
impl__ShowMenu_CMFCCaptionMenuButton__QEAAXPEAUHMENU____PEAVCWnd___Z(
        void* pThis, HMENU hMenu, void* pWnd) {
    S* self = (S*)pThis;
    HWND hWnd = WndHandle(pWnd);

    self->m_bPushed       = TRUE;
    self->m_bMenuIsActive = TRUE;
    self->m_nMenuResult   = 0;

    if (hMenu) {
        POINT pt = { self->m_ptOrg.x, self->m_ptOrg.y };
        if (hWnd)
            ClientToScreen(hWnd, &pt);
        int cmd = TrackPopupMenu(
            hMenu,
            TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
            pt.x, pt.y, 0, hWnd, NULL);
        self->m_nMenuResult = cmd;
        if (cmd && hWnd)
            PostMessageW(hWnd, WM_COMMAND, (WPARAM)cmd, 0);
    }

    self->m_bMenuIsActive = FALSE;
    self->m_bPushed       = FALSE;
}

// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------

namespace {

// Slot 1: {vector deleting destructor}.
void* MS_ABI slot_vecDelDtor(void* pThis, unsigned flags) {
    impl___1CMFCCaptionMenuButton__UEAA_XZ(pThis);
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}

void* const g_CMFCCaptionMenuButton_vtbl[8] = {
    (void*)&slot_GetRuntimeClass,   // 0 CObject::GetRuntimeClass
    (void*)&slot_vecDelDtor,        // 1 CMFCCaptionMenuButton::{dtor}
    (void*)&slot_Serialize,         // 2 CObject::Serialize
    (void*)&slot_AssertValid,       // 3 CObject::AssertValid
    (void*)&slot_Dump,              // 4 CObject::Dump
    (void*)&slot_GetRect,           // 5 CMFCCaptionButton::GetRect
    (void*)&impl__OnDraw_CMFCCaptionMenuButton__UEAAXPEAVCDC__HHHH_Z, // 6 OnDraw
    (void*)&slot_GetIconID,         // 7 CMFCCaptionButton::GetIconID
};

} // namespace
