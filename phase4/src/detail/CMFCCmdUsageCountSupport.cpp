#include "CMFCCmdUsageCountSupport.h"

static_assert(offsetof(CU, m_nTotalCount) == 0x40, "CMFCCmdUsageCount m_nTotalCount @0x40");
static_assert(sizeof(CU) == 0x48, "CMFCCmdUsageCount mirror covers 0x40..0x48");
