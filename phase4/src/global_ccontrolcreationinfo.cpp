// OpenMFC real implementation of CControlCreationInfo.
// Layout-faithful, MS_ABI thunks aliased from the .def via impl_ symbols.
#include <windows.h>
#include <cstddef>   // offsetof
#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// enum HandleKind { ReflectionType, ControlInstance, NullHandle };
enum HandleKind { ReflectionType = 0, ControlInstance = 1, NullHandle = 2 };

// class CControlCreationInfo size(32):
//  0  | HandleKind m_hk        (4 bytes + 4 alignment pad)
//  8  | intptr_t   m_nHandle   (8 bytes)
// 16  | CLSID      m_clsid     (16 bytes)
struct S {
    int      m_hk;        // offset 0 (HandleKind stored as int)
    char     _pad0[4];    // alignment to 8
    intptr_t m_nHandle;   // offset 8
    CLSID    m_clsid;     // offset 16
};

static_assert(sizeof(S) == 32, "CControlCreationInfo size mismatch");
static_assert(offsetof(S, m_hk) == 0, "m_hk offset");
static_assert(offsetof(S, m_nHandle) == 8, "m_nHandle offset");
static_assert(offsetof(S, m_clsid) == 16, "m_clsid offset");

} // namespace

// Symbol: ??0CControlCreationInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CControlCreationInfo__QEAA_XZ(void* pThis)
{
    S* self = reinterpret_cast<S*>(pThis);
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
    const S* self = reinterpret_cast<const S*>(pThis);
    return (self->m_hk != NullHandle) ? TRUE : FALSE;
}
