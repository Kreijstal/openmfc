#include "CMFCTasksPaneSupport.h"

static_assert(offsetof(S_MfcFeature20Impl, m_bCanCollapse)          == 0x4E0, "m_bCanCollapse @0x4E0");
static_assert(offsetof(S_MfcFeature20Impl, m_bHistoryMenuButtons)   == 0x4E8, "m_bHistoryMenuButtons @0x4E8");
static_assert(offsetof(S_MfcFeature20Impl, m_bMenuBtnPressed)       == 0x4F8, "m_bMenuBtnPressed @0x4F8");
static_assert(offsetof(S_MfcFeature20Impl, m_iActivePage)           == 0x504, "m_iActivePage @0x504");
static_assert(offsetof(S_MfcFeature20Impl, m_nVertScrollTotal)      == 0x514, "m_nVertScrollTotal @0x514");
static_assert(offsetof(S_MfcFeature20Impl, m_hFont)                 == 0x550, "m_hFont @0x550");
static_assert(offsetof(S_MfcFeature20Impl, m_rectScrollUp)          == 0x5B8, "m_rectScrollUp @0x5B8");
static_assert(offsetof(S_MfcFeature20Impl, m_rectScrollDn)          == 0x5C8, "m_rectScrollDn @0x5C8");
static_assert(offsetof(S_MfcFeature20Impl, m_rectTasks)             == 0x5D8, "m_rectTasks @0x5D8");
static_assert(offsetof(S_MfcFeature20Impl, m_pHotTask)              == 0x5F8, "m_pHotTask @0x5F8");
static_assert(offsetof(S_MfcFeature20Impl, m_pClickedTask)          == 0x600, "m_pClickedTask @0x600");
static_assert(offsetof(S_MfcFeature20Impl, m_strCaption)            == 0x678, "m_strCaption @0x678");
static_assert(offsetof(S_MfcFeature20Impl, m_pHistoryData)          == 0x780, "m_pHistoryData @0x780");
static_assert(offsetof(S_MfcFeature20Impl, m_nHistorySize)          == 0x788, "m_nHistorySize @0x788");
static_assert(offsetof(S_MfcFeature20Impl, m_pHotGroupCaption)      == 0x7A0, "m_pHotGroupCaption @0x7A0");
static_assert(offsetof(S_MfcFeature20Impl, m_pClickedGroupCaption)  == 0x7A8, "m_pClickedGroupCaption @0x7A8");
namespace openmfc { namespace detail { namespace cmfctaskspane {
void RefreshPane(CMFCTasksPane* pThis)
{
    if (!pThis) return;
    pThis->RecalcLayout();
    pThis->RedrawWindow(nullptr, nullptr, 0x105);
}
} } }  // namespace openmfc::detail::cmfctaskspane
