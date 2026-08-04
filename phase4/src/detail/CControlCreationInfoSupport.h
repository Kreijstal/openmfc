#pragma once
// Shared internals of the former global_ccontrolcreationinfo.cpp translation unit.
// Definitions live in detail/CControlCreationInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ccontrolcreationinfo {} } }
using namespace openmfc::detail::ccontrolcreationinfo;
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


// enum HandleKind { ReflectionType, ControlInstance, NullHandle };
namespace openmfc { namespace detail { namespace ccontrolcreationinfo {
enum HandleKind { ReflectionType = 0, ControlInstance = 1, NullHandle = 2 };
} } }

// class CControlCreationInfo size(32):
//  0  | HandleKind m_hk        (4 bytes + 4 alignment pad)
//  8  | intptr_t   m_nHandle   (8 bytes)
// 16  | CLSID      m_clsid     (16 bytes)
namespace openmfc { namespace detail { namespace ccontrolcreationinfo {
struct S_Ccontrolcreationinfo {
    int      m_hk;        // offset 0 (HandleKind stored as int)
    char     _pad0[4];    // alignment to 8
    intptr_t m_nHandle;   // offset 8
    CLSID    m_clsid;     // offset 16
};
} } }




