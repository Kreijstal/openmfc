#include "CChevronOwnerDrawMenuSupport.h"

// Implementations this unit calls that are defined with their own class.
// This unit only needs afx.h, so CMenu is named by a forward declaration.
// Implementations this unit calls that are defined with their own class.
// This unit only needs afx.h, so CMenu is named by a forward declaration.
class CMenu;

static_assert(sizeof(S_Cchevronownerdrawmenu) == 32, "CChevronOwnerDrawMenu must be 32 bytes");
static_assert(offsetof(S_Cchevronownerdrawmenu, m_hMenu) == 8, "m_hMenu at 8");
static_assert(offsetof(S_Cchevronownerdrawmenu, font_hObject) == 24, "m_MenuFont.m_hObject at 24");
namespace openmfc { namespace detail { namespace cchevronownerdrawmenu {
HFONT EnsureMenuFont_Cchevronownerdrawmenu(S_Cchevronownerdrawmenu* self) {
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
void* MS_ABI slot_GetRuntimeClass_Cchevronownerdrawmenu(const void* pThis) {
    return impl__GetRuntimeClass_CMenu__UEBAPEAUCRuntimeClass__XZ(pThis);
}
void MS_ABI slot_Serialize_Cchevronownerdrawmenu(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI slot_AssertValid_Cchevronownerdrawmenu(const void* pThis) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI slot_Dump_Cchevronownerdrawmenu(const void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
void* MS_ABI slot_vecDelDtor_Cchevronownerdrawmenu(void* pThis, unsigned flags) {
    S_Cchevronownerdrawmenu* self = (S_Cchevronownerdrawmenu*)pThis;
    if (self->font_hObject) {
        DeleteObject(self->font_hObject);
        self->font_hObject = NULL;
    }
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}
} } }  // namespace openmfc::detail::cchevronownerdrawmenu
