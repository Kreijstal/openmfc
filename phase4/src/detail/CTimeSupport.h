#pragma once
// Shared internals of the former global_ctime_serialization.cpp translation unit.
// Definitions live in detail/CTimeSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ctime {} } }
using namespace openmfc::detail::ctime;
// ATL CTime and CTimeSpan archive serialization.

#include <cstdint>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

struct CArchive;


// Both ATL value types contain a single __time64_t value in the MSVC ABI.
namespace openmfc { namespace detail { namespace ctime {
struct CArchiveLayout {
    void* vtable;
    void* file;
    std::uint32_t mode;
};
} } }

extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(
    CArchive* archive,
    void* buffer,
    unsigned int byteCount
);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(
    CArchive* archive,
    const void* buffer,
    unsigned int byteCount
);

namespace openmfc { namespace detail { namespace ctime {
CArchive* SerializeTime64(void* value, CArchive* archive);
} } }



