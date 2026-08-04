#include "CMFCAutoHideButtonSupport.h"

static_assert(offsetof(CAutoHideButton, m_nShow) == 0x0c, "show @0x0c");
static_assert(offsetof(CAutoHideButton, m_dwStyle) == 0x14, "style @0x14");
static_assert(offsetof(CAutoHideButton, m_rect) == 0x1c, "rect @0x1c");
static_assert(offsetof(CAutoHideButton, m_pAutoHideBar) == 0x30, "bar @0x30");
static_assert(offsetof(CAutoHideButton, m_pPane) == 0x38, "pane @0x38");
