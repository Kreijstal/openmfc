// Compatibility extension for looking up fields by name.
#define OPENMFC_APPCORE_IMPL
#include "detail/CRecordsetSupport.h"
void CFieldExchange::SetNull(const wchar_t* name) {
    if(!m_prs || !name)return;
    LoadRecordsetFields(m_prs);
    int index=FindFieldByName(m_prs,name);
    if(index>=0)EnsureRecordsetState(m_prs).nullFields.insert(static_cast<unsigned long>(index));
}
