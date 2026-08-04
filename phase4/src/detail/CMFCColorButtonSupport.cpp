#include "CMFCColorButtonSupport.h"

static_assert(sizeof(CPalHolder_Cmfccolorbutton) == 0x10, "CPalHolder_Cmfccolorbutton must be 0x10 bytes");
static_assert(sizeof(CColorBtn) == 0xbc8, "CMFCColorButton mirror must be 0xBC8 bytes");
static_assert(offsetof(CColorBtn, m_hWnd) == 0x40, "m_hWnd @0x40");
static_assert(offsetof(CColorBtn, m_nStyle) == 0xe8, "m_nStyle @0xe8");
static_assert(offsetof(CColorBtn, m_nColor) == 0xb28, "m_nColor @0xb28");
static_assert(offsetof(CColorBtn, m_clrAutomaticButton) == 0xb2c, "auto color @0xb2c");
static_assert(offsetof(CColorBtn, m_pColorPopup) == 0xb98, "popup @0xb98");
static_assert(offsetof(CColorBtn, m_strDocLabel) == 0xba8, "doc label @0xba8");
static_assert(offsetof(CColorBtn, m_pPaletteObj) == 0xbc0, "palette holder @0xbc0");
