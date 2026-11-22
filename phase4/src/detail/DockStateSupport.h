#pragma once

#include "detail/CControlBarInfoSupport.h"

class CDockState;
extern "C" void* impl___0CDockState__QEAA_XZ(void*);
extern "C" void impl___1CDockState__UEAA_XZ(void*);
extern "C" void impl__Clear_CDockState__QEAAXXZ(void*);
extern "C" void impl__SaveState_CDockState__QEAAXPEB_W_Z(void*, const wchar_t*);
extern "C" void impl__LoadState_CDockState__QEAAXPEB_W_Z(void*, const wchar_t*);

namespace openmfc::detail::dock {
struct DockStateLayout {
    void*    vfptr;               // 0x00
    CPtrArray bars;               // 0x08
    int      m_bScaling;          // 0x30
    RECT     m_rectDevice;        // 0x34
    RECT     m_rectClip;          // 0x44
    SIZE     m_sizeLogical;       // 0x54
    DWORD    m_dwVersion;         // 0x5c
};
static_assert(offsetof(DockStateLayout, bars) == 0x08, "CDockState::m_arrBarInfo");
static_assert(offsetof(DockStateLayout, m_bScaling)    == 0x30, "CDockState::m_bScaling");
static_assert(offsetof(DockStateLayout, m_rectDevice)  == 0x34, "CDockState::m_rectDevice");
static_assert(offsetof(DockStateLayout, m_rectClip)    == 0x44, "CDockState::m_rectClip");
static_assert(offsetof(DockStateLayout, m_sizeLogical) == 0x54, "CDockState::m_sizeLogical");
static_assert(offsetof(DockStateLayout, m_dwVersion)   == 0x5c, "CDockState::m_dwVersion");
static_assert(sizeof(DockStateLayout) == 0x60, "CDockState size (retail CreateObject allocates 0x60)");

inline const CPtrArray& Bars(const CDockState& state) {
    return reinterpret_cast<const DockStateLayout&>(state).bars;
}
inline CPtrArray& Bars(CDockState& state) {
    return reinterpret_cast<DockStateLayout&>(state).bars;
}

class ScopedDockState {
public:
    ScopedDockState() { impl___0CDockState__QEAA_XZ(storage); }
    ~ScopedDockState() { impl___1CDockState__UEAA_XZ(storage); }
    ScopedDockState(const ScopedDockState&) = delete;
    ScopedDockState& operator=(const ScopedDockState&) = delete;
    CDockState& Get() { return *reinterpret_cast<CDockState*>(storage); }
private:
    alignas(DockStateLayout) BYTE storage[sizeof(DockStateLayout)];
};

S_Ccontrolbarinfo* NewBarInfo();
void DeleteBarInfo(S_Ccontrolbarinfo* info);
}
