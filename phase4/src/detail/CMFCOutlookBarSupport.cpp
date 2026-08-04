#include "CMFCOutlookBarSupport.h"

static_assert(sizeof(OutlookBar) == 0x568, "CMFCOutlookBar size mismatch");
static_assert(offsetof(OutlookBar, m_hWnd) == 0x40, "CMFCOutlookBar::m_hWnd offset");
