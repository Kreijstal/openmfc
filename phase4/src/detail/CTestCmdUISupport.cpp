#include "CTestCmdUISupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn);
extern "C" void MS_ABI impl__SetCheck_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int nCheck);
extern "C" void MS_ABI impl__SetRadio_CTestCmdUI__UEAAXH_Z(CCmdUI* pThis, int bOn);
extern "C" void MS_ABI impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(CCmdUI* pThis, const wchar_t* lpszText);

static_assert(sizeof(CCmdUIShim_Ctestcmdui) == 56, "CCmdUIShim_Ctestcmdui size must be 56");
static_assert(offsetof(CCmdUIShim_Ctestcmdui, m_nID) == 8, "m_nID @8");
static_assert(offsetof(CCmdUIShim_Ctestcmdui, m_pOther) == 16, "m_pOther @16");
static_assert(offsetof(CCmdUIShim_Ctestcmdui, m_nIndex) == 24, "m_nIndex @24");
static_assert(offsetof(CCmdUIShim_Ctestcmdui, m_pMenu) == 32, "m_pMenu @32");
static_assert(offsetof(CCmdUIShim_Ctestcmdui, m_pSubMenu) == 40, "m_pSubMenu @40");

namespace openmfc { namespace detail { namespace ctestcmdui {
void MS_ABI vt_Enable_Ctestcmdui(void* pThis, int bOn) {
    impl__Enable_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), bOn);
}
void MS_ABI vt_SetCheck_Ctestcmdui(void* pThis, int nCheck) {
    impl__SetCheck_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), nCheck);
}
void MS_ABI vt_SetRadio_Ctestcmdui(void* pThis, int bOn) {
    impl__SetRadio_CTestCmdUI__UEAAXH_Z(static_cast<CCmdUI*>(pThis), bOn);
}
void MS_ABI vt_SetText_Ctestcmdui(void* pThis, const wchar_t* text) {
    impl__SetText_CTestCmdUI__UEAAXPEB_W_Z(static_cast<CCmdUI*>(pThis), text);
}
} } }  // namespace openmfc::detail::ctestcmdui
