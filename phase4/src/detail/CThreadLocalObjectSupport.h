#pragma once
#include "detail/CThreadSlotDataSupport.h"
struct S_Cthreadlocalobject {
    int m_nSlot;
};
extern "C" void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
namespace openmfc {
namespace detail {
namespace cthreadlocalobject {
extern CThreadSlotData* threadSlots;
extern DWORD tlsReferences;
CThreadSlotData* InitializeThreadSlots();
} // namespace cthreadlocalobject
} // namespace detail
} // namespace openmfc
