#include "Ole09MsgmapSupport.h"

const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWnd() { return &classCOleIPFrameWnd_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_COleDocIPFrameWndEx() { return &classCOleDocIPFrameWnd_msgmap; }
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWndEx() { return &classCOleIPFrameWnd_msgmap; }
namespace openmfc { namespace detail { namespace ole09msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Ole09Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_COleCntrFrameWndEx() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleControl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleDBRecordView() { return impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleIPFrameWnd() { return impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COlePropertyPage() { return impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCOleCntrFrameWndEx_msgmap = { gbm_COleCntrFrameWndEx, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleControl_msgmap = { gbm_COleControl, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleDBRecordView_msgmap = { gbm_COleDBRecordView, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleDocIPFrameWnd_msgmap = { gbm_COleDocIPFrameWnd, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleDocIPFrameWndEx_msgmap = { gbm_COleDocIPFrameWndEx, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleIPFrameWnd_msgmap = { gbm_COleIPFrameWnd, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOleIPFrameWndEx_msgmap = { gbm_COleIPFrameWndEx, g_emptyMsgEntries_Ole09Msgmap };
const AFX_MSGMAP classCOlePropertyPage_msgmap = { gbm_COlePropertyPage, g_emptyMsgEntries_Ole09Msgmap };
} } }  // namespace openmfc::detail::ole09msgmap
