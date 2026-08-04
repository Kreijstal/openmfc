#include "CControlCreationInfoSupport.h"

static_assert(sizeof(S_Ccontrolcreationinfo) == 32, "CControlCreationInfo size mismatch");
static_assert(offsetof(S_Ccontrolcreationinfo, m_hk) == 0, "m_hk offset");
static_assert(offsetof(S_Ccontrolcreationinfo, m_nHandle) == 8, "m_nHandle offset");
static_assert(offsetof(S_Ccontrolcreationinfo, m_clsid) == 16, "m_clsid offset");
