#include "Pane15MsgmapSupport.h"
#include "openmfc/afxmfc.h"

const AFX_MSGMAP* AFXAPI gbm_CAutoHideDockSite() { return &classCDockSite_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CBaseTabbedPane() { return &classCDockablePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDockSite() { return &classCBasePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDockablePaneAdapter() { return &classCDockablePane_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDummyDockablePane() { return &classCDockablePane_msgmap; }
namespace openmfc { namespace detail { namespace pane15msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane15Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CBasePane() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDockablePane() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCAutoHideDockSite_msgmap = { gbm_CAutoHideDockSite, g_emptyMsgEntries_Pane15Msgmap };
const AFX_MSGMAP classCBasePane_msgmap = { gbm_CBasePane, g_emptyMsgEntries_Pane15Msgmap };
namespace {
const AFX_MSGMAP_ENTRY baseTabbedPaneEntries[] = {
    OPENMFC_MAP_ENTRY(WM_SIZE, 0, 0, 0, AfxSig_v_u_ii, &CBaseTabbedPane::OnSize)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCBaseTabbedPane_msgmap = { gbm_CBaseTabbedPane, baseTabbedPaneEntries };
const AFX_MSGMAP classCDockSite_msgmap = { gbm_CDockSite, g_emptyMsgEntries_Pane15Msgmap };
namespace {
using ThisClass = CDockablePane;
const AFX_MSGMAP_ENTRY dockablePaneEntries[] = {
    ON_WM_TIMER()
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCDockablePane_msgmap = { gbm_CDockablePane, dockablePaneEntries };
const AFX_MSGMAP classCDockablePaneAdapter_msgmap = { gbm_CDockablePaneAdapter, g_emptyMsgEntries_Pane15Msgmap };
const AFX_MSGMAP classCDummyDockablePane_msgmap = { gbm_CDummyDockablePane, g_emptyMsgEntries_Pane15Msgmap };
} } }  // namespace openmfc::detail::pane15msgmap
