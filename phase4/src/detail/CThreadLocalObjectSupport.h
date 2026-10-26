#pragma once
#include "detail/CThreadSlotDataSupport.h"
struct S_Cthreadlocalobject {
    int m_nSlot;
};
namespace openmfc {
namespace detail {
namespace cthreadlocalobject {
extern CThreadSlotData* threadSlots;
extern DWORD tlsReferences;
CThreadSlotData* InitializeThreadSlots();
} // namespace cthreadlocalobject
} // namespace detail
} // namespace openmfc
