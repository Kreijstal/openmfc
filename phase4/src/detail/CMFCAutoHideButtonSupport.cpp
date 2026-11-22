#include "CMFCAutoHideButtonSupport.h"

static_assert(sizeof(CMFCAutoHideButton) == 0x40);
static_assert(offsetof(CMFCAutoHideButton, m_bVisible) == 0x0c);
static_assert(offsetof(CMFCAutoHideButton, m_dwAlignment) == 0x14);
static_assert(offsetof(CMFCAutoHideButton, m_rect) == 0x1c);
static_assert(offsetof(CMFCAutoHideButton, m_pParentBar) == 0x30);
static_assert(offsetof(CMFCAutoHideButton, m_pAutoHideWindow) == 0x38);
