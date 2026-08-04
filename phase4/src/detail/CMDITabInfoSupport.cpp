#include "CMDITabInfoSupport.h"

static_assert(sizeof(S_Cmditabinfo) == 48, "CMDITabInfo must be 48 bytes");
static_assert(offsetof(S_Cmditabinfo, m_tabLocation) == 0, "m_tabLocation @0");
static_assert(offsetof(S_Cmditabinfo, m_style) == 4, "m_style @4");
static_assert(offsetof(S_Cmditabinfo, m_bTabIcons) == 8, "m_bTabIcons @8");
static_assert(offsetof(S_Cmditabinfo, m_nTabBorderSize) == 40, "m_nTabBorderSize @40");
static_assert(offsetof(S_Cmditabinfo, m_bReuseRemovedTabGroups) == 44, "m_bReuseRemovedTabGroups @44");
static_assert(offsetof(ArchiveView, m_nMode) == 16, "CArchive::m_nMode @16");
