#include "CMFCToolBarInfoSupport.h"

static_assert(sizeof(S_Cmfctoolbarinfo) == 32, "CMFCToolBarInfo size mismatch");
static_assert(offsetof(S_Cmfctoolbarinfo, m_uiColdResID) == 0, "m_uiColdResID offset");
static_assert(offsetof(S_Cmfctoolbarinfo, m_uiHotResID) == 4, "m_uiHotResID offset");
static_assert(offsetof(S_Cmfctoolbarinfo, m_uiDisabledResID) == 8, "m_uiDisabledResID offset");
static_assert(offsetof(S_Cmfctoolbarinfo, m_uiMenuDisabledResID) == 28, "m_uiMenuDisabledResID offset");
