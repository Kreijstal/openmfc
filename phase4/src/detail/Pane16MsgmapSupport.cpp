#include "Pane16MsgmapSupport.h"
#include "openmfc/afxmfc.h"

const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd() { return &classCPaneFrameWnd_msgmap; }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane16Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CPane() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDialog() { return impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDivider() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneFrameWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneTrackingWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingGroupGuidesWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCMultiPaneFrameWnd_msgmap = { gbm_CMultiPaneFrameWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPane_msgmap = { gbm_CPane, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneDialog_msgmap = { gbm_CPaneDialog, g_emptyMsgEntries_Pane16Msgmap };
namespace {
using ThisClass = CPaneDivider;
const AFX_MSGMAP_ENTRY dividerEntries[] = {
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_SETCURSOR()
    OPENMFC_MAP_ENTRY(WM_CANCELMODE, 0, 0, 0, AfxSig_v_v_v, &ThisClass::OnCancelMode)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCPaneDivider_msgmap = { gbm_CPaneDivider, dividerEntries };
const AFX_MSGMAP classCPaneFrameWnd_msgmap = { gbm_CPaneFrameWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneTrackingWnd_msgmap = { gbm_CPaneTrackingWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCSmartDockingGroupGuidesWnd_msgmap = { gbm_CSmartDockingGroupGuidesWnd, g_emptyMsgEntries_Pane16Msgmap };
} } }  // namespace openmfc::detail::pane16msgmap
