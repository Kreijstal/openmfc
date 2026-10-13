// Shared state and size bounds for ABI collection wrappers.
#include <mutex>
#include <windows.h>
#include <limits>

namespace openmfc { namespace detail { namespace filecore {
std::mutex g_collectionStateMutex;
int ClampCollectionSize(INT_PTR value) {
    if (value <= 0) return 0;
    if (value > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(value);
}
int ClampCollectionBlockSize(INT_PTR value) {
    if (value <= 0) return 10;
    if (value > std::numeric_limits<int>::max()) return std::numeric_limits<int>::max();
    return static_cast<int>(value);
}
} } }
