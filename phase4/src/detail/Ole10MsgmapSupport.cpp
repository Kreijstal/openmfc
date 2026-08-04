#include "Ole10MsgmapSupport.h"

const AFX_MSGMAP* AFXAPI gbm_COleServerDocEx() { return &classCOleServerDoc_msgmap; }
namespace openmfc { namespace detail { namespace ole10msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Ole10Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_COleResizeBar() { return impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_COleServerDoc() { return impl__GetThisMessageMap_CDocument__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCOleResizeBar_msgmap = { gbm_COleResizeBar, g_emptyMsgEntries_Ole10Msgmap };
const AFX_MSGMAP classCOleServerDoc_msgmap = { gbm_COleServerDoc, g_emptyMsgEntries_Ole10Msgmap };
const AFX_MSGMAP classCOleServerDocEx_msgmap = { gbm_COleServerDocEx, g_emptyMsgEntries_Ole10Msgmap };
} } }  // namespace openmfc::detail::ole10msgmap
