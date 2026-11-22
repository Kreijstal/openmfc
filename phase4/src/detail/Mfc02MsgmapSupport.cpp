#include "Mfc02MsgmapSupport.h"
#include "openmfc/afxmfc.h"

const AFX_MSGMAP* AFXAPI gbm_CMFCColorButton() { return &classCMFCButton_msgmap; }
namespace openmfc { namespace detail { namespace mfc02msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Mfc02Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CMFCAcceleratorKeyAssignCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCAutoHideBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseTabCtrl() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCBaseToolBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCButton() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCCaptionBar() { return impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CMFCColorBar() { return impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP classCMFCAcceleratorKeyAssignCtrl_msgmap = { gbm_CMFCAcceleratorKeyAssignCtrl, g_emptyMsgEntries_Mfc02Msgmap };
namespace {
using ThisClass = CMFCAutoHideBar;
const AFX_MSGMAP_ENTRY autoHideBarEntries[] = {
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    OPENMFC_MAP_ENTRY(WM_MOUSELEAVE, 0, 0, 0, AfxSig_v_v_v, &ThisClass::OnMouseLeave)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCMFCAutoHideBar_msgmap = { gbm_CMFCAutoHideBar, autoHideBarEntries };
const AFX_MSGMAP classCMFCBaseTabCtrl_msgmap = { gbm_CMFCBaseTabCtrl, g_emptyMsgEntries_Mfc02Msgmap };
const AFX_MSGMAP classCMFCBaseToolBar_msgmap = { gbm_CMFCBaseToolBar, g_emptyMsgEntries_Mfc02Msgmap };
const AFX_MSGMAP classCMFCButton_msgmap = { gbm_CMFCButton, g_emptyMsgEntries_Mfc02Msgmap };
const AFX_MSGMAP classCMFCCaptionBar_msgmap = { gbm_CMFCCaptionBar, g_emptyMsgEntries_Mfc02Msgmap };
const AFX_MSGMAP classCMFCColorBar_msgmap = { gbm_CMFCColorBar, g_emptyMsgEntries_Mfc02Msgmap };
const AFX_MSGMAP classCMFCColorButton_msgmap = { gbm_CMFCColorButton, g_emptyMsgEntries_Mfc02Msgmap };
} } }  // namespace openmfc::detail::mfc02msgmap
