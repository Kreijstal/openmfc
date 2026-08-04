#include "Other11MsgmapSupport.h"

const AFX_MSGMAP* AFXAPI gbm_CDialogBar() { return &classCControlBar_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_CDocObjectServer() { return &classCCmdTarget_msgmap; }
namespace openmfc { namespace detail { namespace other11msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Other11Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CCommonDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CControlBar() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CCtrlView() { return impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CDHtmlDialog() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CHtmlEditView() { return impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCCmdTarget_msgmap = { nullptr, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCCommonDialog_msgmap = { gbm_CCommonDialog, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCControlBar_msgmap = { gbm_CControlBar, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCCtrlView_msgmap = { gbm_CCtrlView, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCDHtmlDialog_msgmap = { gbm_CDHtmlDialog, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCDialogBar_msgmap = { gbm_CDialogBar, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCDocObjectServer_msgmap = { gbm_CDocObjectServer, g_emptyMsgEntries_Other11Msgmap };
const AFX_MSGMAP classCHtmlEditView_msgmap = { gbm_CHtmlEditView, g_emptyMsgEntries_Other11Msgmap };
} } }  // namespace openmfc::detail::other11msgmap
