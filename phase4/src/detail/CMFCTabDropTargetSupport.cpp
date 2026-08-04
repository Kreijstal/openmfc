#include "CMFCTabDropTargetSupport.h"

static_assert(sizeof(S_Cmfctabdroptarget) == 112,                     "CMFCTabDropTarget size");
static_assert(offsetof(S_Cmfctabdroptarget, m_dwRef) == 8,            "m_dwRef offset");
static_assert(offsetof(S_Cmfctabdroptarget, m_hWnd) == 64,            "m_hWnd offset");
static_assert(offsetof(S_Cmfctabdroptarget, m_xDropTarget) == 96,     "m_xDropTarget offset");
static_assert(offsetof(S_Cmfctabdroptarget, m_pOwner) == 104,         "m_pOwner offset");
namespace openmfc { namespace detail { namespace cmfctabdroptarget {
void** OwnerVtbl(void* owner) { return *reinterpret_cast<void***>(owner); }
} } }  // namespace openmfc::detail::cmfctabdroptarget
