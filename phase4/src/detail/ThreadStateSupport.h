#pragma once
#include "openmfc/afxstat_.h"
namespace openmfc { namespace detail { namespace threadstate {
// Look up the process thread-state slot without allocating a value.
_AFX_THREAD_STATE* Existing();
} } }
