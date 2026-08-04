#include "CMFCTasksPanePropertyPageSupport.h"

static_assert(sizeof(S_Cmfctaskspanepropertypage) == 24, "CMFCTasksPanePropertyPage size must match harvested layout");
static_assert(offsetof(S_Cmfctaskspanepropertypage, m_strName) == 8,   "m_strName @8");
static_assert(offsetof(S_Cmfctaskspanepropertypage, m_pTaskPane) == 16, "m_pTaskPane @16");
