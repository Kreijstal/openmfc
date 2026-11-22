#include "Pane17MsgmapSupport.h"
#include "openmfc/afxmfc.h"

namespace openmfc { namespace detail { namespace pane17msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane17Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingHighlighterWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingStandaloneGuideWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CTabbedPane() { return impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCSmartDockingHighlighterWnd_msgmap = { gbm_CSmartDockingHighlighterWnd, g_emptyMsgEntries_Pane17Msgmap };
const AFX_MSGMAP classCSmartDockingStandaloneGuideWnd_msgmap = { gbm_CSmartDockingStandaloneGuideWnd, g_emptyMsgEntries_Pane17Msgmap };
namespace {
using ThisClass = CTabbedPane;
const AFX_MSGMAP_ENTRY tabbedPaneEntries[] = {
    ON_WM_CREATE()
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCTabbedPane_msgmap = { gbm_CTabbedPane, tabbedPaneEntries };
} } }  // namespace openmfc::detail::pane17msgmap
