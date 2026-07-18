// OpenMFC: CChevronOwnerDrawMenu  (public CMenu : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(32):
//   0  {vfptr}          (CObject/CMenu vfptr)
//   8  m_hMenu          (CMenu::m_hMenu, HMENU)
//  16  m_MenuFont       (CFont : CGdiObject : CObject) -> {vfptr, m_hObject}
//        16  m_MenuFont.{vfptr}
//        24  m_MenuFont.m_hObject   (HFONT)
//
// This is the internal owner-draw menu used to render the ">>" chevron
// overflow popup.  MeasureItem sizes each item from its text using the menu
// font; DrawItem paints the background per selection state and draws the text.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

struct S {
    void*   vfptr;             // 0
    HMENU   m_hMenu;           // 8
    void*   font_vfptr;        // 16  m_MenuFont.{vfptr}
    HGDIOBJ font_hObject;      // 24  m_MenuFont.m_hObject (HFONT)
};

static_assert(sizeof(S) == 32, "CChevronOwnerDrawMenu must be 32 bytes");
static_assert(offsetof(S, m_hMenu) == 8, "m_hMenu at 8");
static_assert(offsetof(S, font_hObject) == 24, "m_MenuFont.m_hObject at 24");

// Lazily create the menu font from the current non-client metrics and cache it
// in the CFont subobject (m_MenuFont.m_hObject).  Returns a usable HFONT.
static HFONT EnsureMenuFont(S* self) {
    if (self->font_hObject)
        return (HFONT)self->font_hObject;

    NONCLIENTMETRICSW ncm;
    ZeroMemory(&ncm, sizeof(ncm));
    ncm.cbSize = sizeof(ncm);
    HFONT hf = NULL;
    if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0))
        hf = CreateFontIndirectW(&ncm.lfMenuFont);
    if (!hf)
        hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    self->font_hObject = (HGDIOBJ)hf;
    return hf;
}

// ---- vtable slot helpers (non-exported virtuals) ----------------------------

// Slot 0: CMenu::GetRuntimeClass  — delegate to the real CMenu impl so the
// object reports RuntimeClass "CMenu" (this class is not DECLARE_DYNAMIC).
extern "C" void* MS_ABI
impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

static void* MS_ABI slot_GetRuntimeClass(const void* pThis) {
    return impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(pThis);
}

// Slot 2: CObject::Serialize — base implementation is a no-op.
static void MS_ABI slot_Serialize(void* /*pThis*/, void* /*ar*/) {}

// Slot 3: CObject::AssertValid — const, no-op in release.
static void MS_ABI slot_AssertValid(const void* /*pThis*/) {}

// Slot 4: CObject::Dump — const, no-op in release.
static void MS_ABI slot_Dump(const void* /*pThis*/, void* /*dc*/) {}

} // namespace

// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace { extern void* const g_CChevronOwnerDrawMenu_vtbl[7]; }

// Symbol: ?DrawItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI
impl__DrawItem_CChevronOwnerDrawMenu__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
        void* pThis, DRAWITEMSTRUCT* dis) {
    S* self = (S*)pThis;
    if (!dis || dis->CtlType != ODT_MENU || !dis->hDC)
        return;

    HDC hdc = dis->hDC;
    const RECT& rc = dis->rcItem;
    const bool selected = (dis->itemState & ODS_SELECTED) != 0;
    const bool grayed   = (dis->itemState & (ODS_GRAYED | ODS_DISABLED)) != 0;

    // Background.
    COLORREF bk = GetSysColor(selected ? COLOR_HIGHLIGHT : COLOR_MENU);
    HBRUSH hbr = CreateSolidBrush(bk);
    if (hbr) {
        FillRect(hdc, &rc, hbr);
        DeleteObject(hbr);
    }

    // Text.
    LPCWSTR text = (LPCWSTR)dis->itemData;
    if (text && *text) {
        HFONT hf = EnsureMenuFont(self);
        HGDIOBJ old = SelectObject(hdc, hf);
        int oldBk = SetBkMode(hdc, TRANSPARENT);
        COLORREF fg = GetSysColor(grayed ? COLOR_GRAYTEXT
                                         : (selected ? COLOR_HIGHLIGHTTEXT
                                                     : COLOR_MENUTEXT));
        COLORREF oldFg = SetTextColor(hdc, fg);

        RECT rt = rc;
        rt.left += GetSystemMetrics(SM_CXMENUCHECK) + 2;
        DrawTextW(hdc, text, -1, &rt,
                  DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

        SetTextColor(hdc, oldFg);
        SetBkMode(hdc, oldBk);
        SelectObject(hdc, old);
    }
}

// Symbol: ?MeasureItem@CChevronOwnerDrawMenu@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI
impl__MeasureItem_CChevronOwnerDrawMenu__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
        void* pThis, MEASUREITEMSTRUCT* mis) {
    S* self = (S*)pThis;
    if (!mis || mis->CtlType != ODT_MENU)
        return;

    int cyMenu = GetSystemMetrics(SM_CYMENU);
    int cxCheck = GetSystemMetrics(SM_CXMENUCHECK);

    LPCWSTR text = (LPCWSTR)mis->itemData;
    UINT w = 0, h = (UINT)cyMenu;

    HDC hdc = GetDC(NULL);
    if (hdc) {
        HFONT hf = EnsureMenuFont(self);
        HGDIOBJ old = SelectObject(hdc, hf);
        SIZE sz = {0, 0};
        if (text && *text)
            GetTextExtentPoint32W(hdc, text, (int)wcslen(text), &sz);
        TEXTMETRICW tm;
        if (GetTextMetricsW(hdc, &tm) && (int)h < tm.tmHeight)
            h = (UINT)tm.tmHeight;
        SelectObject(hdc, old);
        ReleaseDC(NULL, hdc);
        w = (UINT)sz.cx;
    }

    // Room for the check/gutter column plus a right margin.
    mis->itemWidth  = w + (UINT)cxCheck + 4;
    mis->itemHeight = h;
}

// Symbol: ??0CChevronOwnerDrawMenu@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CChevronOwnerDrawMenu__QEAA_XZ(void* pThis) {
    S* self = (S*)pThis;
    self->vfptr        = (void*)&g_CChevronOwnerDrawMenu_vtbl[0];
    self->m_hMenu      = NULL;              // CMenu default
    self->font_vfptr   = NULL;              // CFont subobject (m_hObject = NULL)
    self->font_hObject = NULL;
    return pThis;
}

// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------

namespace {

// Slot 1: CChevronOwnerDrawMenu::{vector deleting destructor}.
// Frees the cached menu font, then (if flag bit 0) releases the object.
void* MS_ABI slot_vecDelDtor(void* pThis, unsigned flags) {
    S* self = (S*)pThis;
    if (self->font_hObject) {
        DeleteObject(self->font_hObject);
        self->font_hObject = NULL;
    }
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}

void* const g_CChevronOwnerDrawMenu_vtbl[7] = {
    (void*)&slot_GetRuntimeClass,   // 0 CMenu::GetRuntimeClass
    (void*)&slot_vecDelDtor,        // 1 {dtor}
    (void*)&slot_Serialize,         // 2 CObject::Serialize
    (void*)&slot_AssertValid,       // 3 CObject::AssertValid
    (void*)&slot_Dump,              // 4 CObject::Dump
    (void*)&impl__DrawItem_CChevronOwnerDrawMenu__UEAAXPEAUtagDRAWITEMSTRUCT___Z,   // 5
    (void*)&impl__MeasureItem_CChevronOwnerDrawMenu__UEAAXPEAUtagMEASUREITEMSTRUCT___Z, // 6
};

} // namespace
