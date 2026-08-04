#include "CTimeSupport.h"

namespace openmfc { namespace detail { namespace ctime {
CArchive* SerializeTime64(void* value, CArchive* archive) {
    if (!value || !archive) {
        return archive;
    }

    if ((reinterpret_cast<CArchiveLayout*>(archive)->mode & 1U) != 0) {
        impl__Read_CArchive__QEAAIPEAXI_Z(archive, value, sizeof(long long));
    } else {
        impl__Write_CArchive__QEAAXPEBXI_Z(archive, value, sizeof(long long));
    }
    return archive;
}
} } }  // namespace openmfc::detail::ctime
