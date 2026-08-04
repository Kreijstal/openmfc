#include "CMFCColorBarSupport.h"

static_assert(sizeof(CBar) == 0x1508, "CMFCColorBar mirror must be 0x1508 bytes");
static_assert(offsetof(CBar, m_hWnd) == 0x40, "CMFCColorBar m_hWnd @0x40");
static_assert(offsetof(CBar, m_hWndParent) == 0xa0, "CMFCColorBar m_hWndParent @0xa0");
static_assert(offsetof(CBar, m_nHotIndex) == 0x1138, "CMFCColorBar m_nHotIndex @0x1138");
static_assert(offsetof(CBar, m_nHighlightedIndex) == 0x1140, "CMFCColorBar 0x1140 index");
static_assert(offsetof(CBar, m_pListButtons) == 0x1190, "CMFCColorBar 0x1190 CList");
static_assert(offsetof(CBar, m_nListButtonCount) == 0x11a0, "CMFCColorBar 0x11a0 count");
static_assert(offsetof(CBar, m_bGridSizeFlag) == 0x1448, "CMFCColorBar 0x1448 flag");
static_assert(offsetof(CBar, m_bResetAuto) == 0x144c, "CMFCColorBar 0x144c flag");
static_assert(offsetof(CBar, m_nCommandId) == 0x1454, "CMFCColorBar m_nCommandId @0x1454");
static_assert(offsetof(CBar, m_nColorCount) == 0x1468, "CMFCColorBar m_nColorCount @0x1468");
static_assert(offsetof(CBar, m_nAutoColor) == 0x14d0, "CMFCColorBar m_nAutoColor @0x14d0");
static_assert(offsetof(CBar, m_strAutoLabel) == 0x14d8, "CMFCColorBar 0x14d8 CString");
static_assert(offsetof(CBar, m_strThirdLabel) == 0x14e8, "CMFCColorBar 0x14e8 CString");
static_assert(offsetof(CBar, m_pBtnAuto) == 0x14f0, "CMFCColorBar m_pBtnAuto @0x14f0");
static_assert(offsetof(CBar, m_pBtnOther) == 0x14f8, "CMFCColorBar m_pBtnOther @0x14f8");
static_assert(offsetof(CBar, m_pMenuBar) == 0x1500, "CMFCColorBar m_pMenuBar @0x1500");
