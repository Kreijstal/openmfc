// CMFCToolBarInfo — OpenMFC implementation.
// Sources: global_cmfctoolbarinfo.cpp

#include "detail/CMFCToolBarInfoSupport.h"

// Symbol: ??0CMFCToolBarInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarInfo__QEAA_XZ(void* pThis)
{
    S_Cmfctoolbarinfo* p = static_cast<S_Cmfctoolbarinfo*>(pThis);
    // Default ctor initializes every resource id to 0 (no images assigned).
    p->m_uiColdResID          = 0;
    p->m_uiHotResID           = 0;
    p->m_uiDisabledResID      = 0;
    p->m_uiLargeColdResID     = 0;
    p->m_uiLargeHotResID      = 0;
    p->m_uiLargeDisabledResID = 0;
    p->m_uiMenuResID          = 0;
    p->m_uiMenuDisabledResID  = 0;
    return pThis;
}
