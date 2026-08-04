#include "Ole6ImplSupport.h"

static_assert(offsetof(COleControl, m_pWndOpenFrame) == 280, "m_pWndOpenFrame @280");
static_assert(offsetof(COleControl, m_pReflect) == 336, "m_pReflect @336");
static_assert(offsetof(COleControl, m_nIDTracking) == 344, "m_nIDTracking @344");
static_assert(offsetof(COleControl, m_nIDLastMessage) == 348, "m_nIDLastMessage @348");
static_assert(offsetof(COleControl, m_hFontPrev) == 400, "m_hFontPrev @400");
static_assert(offsetof(COleControl, m_pClientSite) == 472, "m_pClientSite @472");
static_assert(offsetof(COleControl, m_pInPlaceSite) == 480, "m_pInPlaceSite @480");
static_assert(offsetof(COleControl, m_pControlSite) == 488, "m_pControlSite @488");
static_assert(offsetof(COleControl, m_xOleObject) == 616, "m_xOleObject @616");
namespace openmfc { namespace detail { namespace ole6impl {
short GetShiftState()
{
    short nShift = 0;
    if (::GetKeyState(VK_SHIFT) < 0)   nShift |= 1;   // MK_SHIFT
    if (::GetKeyState(VK_CONTROL) < 0) nShift |= 2;   // MK_CONTROL
    if (::GetKeyState(VK_MENU) < 0)    nShift |= 4;   // MK_ALT
    return nShift;
}
} } }  // namespace openmfc::detail::ole6impl
