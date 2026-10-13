#include "CWndSupport.h"

namespace openmfc { namespace detail { namespace cwnd {
std::mutex g_coreWndStateMutex;
std::map<const CWnd*, int> g_coreWndModalResults;
} } }  // namespace openmfc::detail::cwnd
