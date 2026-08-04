#include "Pane16MsgmapSupport.h"

const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd() { return &classCPaneFrameWnd_msgmap; }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane16Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CMiniDockFrameWnd() { return impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPane() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDialog() { return impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDivider() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneFrameWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneTrackingWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingGroupGuidesWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCMiniDockFrameWnd_msgmap = { gbm_CMiniDockFrameWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCMultiPaneFrameWnd_msgmap = { gbm_CMultiPaneFrameWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPane_msgmap = { gbm_CPane, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneDialog_msgmap = { gbm_CPaneDialog, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneDivider_msgmap = { gbm_CPaneDivider, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneFrameWnd_msgmap = { gbm_CPaneFrameWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCPaneTrackingWnd_msgmap = { gbm_CPaneTrackingWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCSmartDockingGroupGuidesWnd_msgmap = { gbm_CSmartDockingGroupGuidesWnd, g_emptyMsgEntries_Pane16Msgmap };
} } }  // namespace openmfc::detail::pane16msgmap
