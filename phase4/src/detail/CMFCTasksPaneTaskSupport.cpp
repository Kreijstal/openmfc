#include "CMFCTasksPaneTaskSupport.h"

static_assert(sizeof(S_Cmfctaskspanetask) == 104, "CMFCTasksPaneTask size must match harvested layout");
static_assert(offsetof(S_Cmfctaskspanetask, m_pGroup) == 8,   "m_pGroup @8");
static_assert(offsetof(S_Cmfctaskspanetask, m_rect) == 24,     "m_rect @24");
static_assert(offsetof(S_Cmfctaskspanetask, m_dwUserData) == 56, "m_dwUserData @56");
static_assert(offsetof(S_Cmfctaskspanetask, m_hwndTask) == 64, "m_hwndTask @64");
static_assert(offsetof(S_Cmfctaskspanetask, m_bIsSeparator) == 84, "m_bIsSeparator @84");
