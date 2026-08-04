#include "CMFCOutlookBarScrollButtonSupport.h"

static_assert(offsetof(CDC_head, m_hDC) == 8, "CDC::m_hDC offset");
static_assert(offsetof(CDC_head, m_hAttribDC) == 16, "CDC::m_hAttribDC offset");
namespace openmfc { namespace detail { namespace cmfcoutlookbarscrollbutton {
HDC DcHandle_Cmfcoutlookbarscrollbutton(void* pDC) {
    return pDC ? static_cast<CDC_head*>(pDC)->m_hDC : nullptr;
}
} } }  // namespace openmfc::detail::cmfcoutlookbarscrollbutton
