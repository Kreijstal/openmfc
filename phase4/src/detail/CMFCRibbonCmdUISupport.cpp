#include "CMFCRibbonCmdUISupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn);
extern "C" void MS_ABI impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int nCheck);
extern "C" void MS_ABI impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn);
extern "C" void MS_ABI impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText);

static_assert(sizeof(S_Cmfcribboncmdui) == 72, "CMFCRibbonCmdUI size must be 72");
static_assert(offsetof(S_Cmfcribboncmdui, m_nID) == 8, "m_nID @8");
static_assert(offsetof(S_Cmfcribboncmdui, m_bEnableChanged) == 44, "m_bEnableChanged @44");
static_assert(offsetof(S_Cmfcribboncmdui, m_pUpdated) == 64, "m_pUpdated @64");

namespace openmfc { namespace detail { namespace cmfcribboncmdui {
void MS_ABI vt_Enable_Cmfcribboncmdui(void* p, int b)  { impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(p, b); }
void MS_ABI vt_SetCheck_Cmfcribboncmdui(void* p, int c){ impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(p, c); }
void MS_ABI vt_SetRadio_Cmfcribboncmdui(void* p, int b){ impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(p, b); }
void MS_ABI vt_SetText_Cmfcribboncmdui(void* p, const wchar_t* t){ impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(p, t); }
} } }  // namespace openmfc::detail::cmfcribboncmdui
