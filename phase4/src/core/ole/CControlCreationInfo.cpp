// CControlCreationInfo — OpenMFC implementation.
// Sources: global_ccontrolcreationinfo.cpp

#include "detail/CControlCreationInfoSupport.h"

// Symbol: ??0CControlCreationInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CControlCreationInfo__QEAA_XZ(void* pThis)
{
    S_Ccontrolcreationinfo* self = reinterpret_cast<S_Ccontrolcreationinfo*>(pThis);
    self->m_hk = NullHandle;
    self->m_nHandle = 0;
    self->m_clsid = GUID_NULL;
    return pThis;
}
// Symbol: ?IsManaged@CControlCreationInfo@@QEBAHXZ
// A creation-info block is "managed" when it carries a live control/reflection
// handle rather than the empty NullHandle default.
extern "C" BOOL MS_ABI impl__IsManaged_CControlCreationInfo__QEBAHXZ(const void* pThis)
{
    const S_Ccontrolcreationinfo* self = reinterpret_cast<const S_Ccontrolcreationinfo*>(pThis);
    return (self->m_hk != NullHandle) ? TRUE : FALSE;
}
