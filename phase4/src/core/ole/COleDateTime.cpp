// COleDateTime — OpenMFC implementation.
// Sources: olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

COleDateTime COleDateTime::GetCurrentTime() {
    COleDateTime dt;
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToVariantTime(&st, &dt.m_dt);
    dt.m_status = DT_VALID;
    return dt;
}
COleDateTime& COleDateTime::operator=(const COleDateTime& dtSrc) {
    m_dt = dtSrc.m_dt;
    m_status = dtSrc.m_status;
    return *this;
}
