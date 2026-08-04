#include "AfxMessagesSupport.h"

namespace openmfc { namespace detail { namespace afxmessages {
unsigned int RegisterAfxMessage(const wchar_t* name)
{
    return ::RegisterWindowMessageW(name);
}
} } }  // namespace openmfc::detail::afxmessages
