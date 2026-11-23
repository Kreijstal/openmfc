#include "Pane16MsgmapSupport.h"
#include "detail/MultiPaneFrameSupport.h"

extern "C" UINT impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA;

const AFX_MSGMAP* AFXAPI gbm_CMultiPaneFrameWnd() { return &classCPaneFrameWnd_msgmap; }
namespace openmfc { namespace detail { namespace pane16msgmap {
const AFX_MSGMAP_ENTRY g_emptyMsgEntries_Pane16Msgmap[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };
const AFX_MSGMAP* AFXAPI gbm_CPane() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDialog() { return impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneDivider() { return impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneFrameWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CPaneTrackingWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }
const AFX_MSGMAP* AFXAPI gbm_CSmartDockingGroupGuidesWnd() { return impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ(); }

namespace {
const AFX_MSGMAP_ENTRY paneEntries[] = {
    OPENMFC_MAP_ENTRY(WM_MOUSEMOVE, 0, 0, 0, AfxSig_v_u_p, &CPane::OnDragMouseMove)
    OPENMFC_MAP_ENTRY(WM_LBUTTONUP, 0, 0, 0, AfxSig_v_u_p, &CPane::OnDragButtonUp)
    OPENMFC_MAP_ENTRY(WM_CANCELMODE, 0, 0, 0, AfxSig_v_v_v, &CPane::OnCancelDrag)
    OPENMFC_MAP_ENTRY(WM_CAPTURECHANGED, 0, 0, 0, AfxSig_l_w_l, &CPane::OnDragCaptureChanged)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
const AFX_MSGMAP classCPane_msgmap = { gbm_CPane, paneEntries };
const AFX_MSGMAP classCPaneDialog_msgmap = { gbm_CPaneDialog, g_emptyMsgEntries_Pane16Msgmap };
namespace {
namespace multi_frame_map {
using ThisClass = CMultiPaneFrameWnd;
const AFX_MSGMAP_ENTRY entries[] = {
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_SIZE()
    OPENMFC_MAP_ENTRY(WM_NCLBUTTONDBLCLK, 0, 0, 0, AfxSig_l_w_l, &ThisClass::OnCaptionDoubleClick)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
namespace divider_map {
using ThisClass = CPaneDivider;
const AFX_MSGMAP_ENTRY dividerEntries[] = {
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_MESSAGE(WM_CAPTURECHANGED, &ThisClass::OnCaptureChanged)
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_SETCURSOR()
    OPENMFC_MAP_ENTRY(WM_CANCELMODE, 0, 0, 0, AfxSig_v_v_v, &ThisClass::OnCancelMode)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
namespace frame_map {
using ThisClass = CPaneFrameWnd;
const AFX_MSGMAP_ENTRY paneFrameEntries[] = {
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_WM_MOVING()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_MESSAGE(WM_ENTERSIZEMOVE, &ThisClass::OnEnterSizeMove)
    ON_MESSAGE(WM_EXITSIZEMOVE, &ThisClass::OnExitSizeMove)
    ON_MESSAGE(WM_NCLBUTTONDOWN, &ThisClass::OnCaptionButtonDown)
    ON_MESSAGE(WM_NCHITTEST, &ThisClass::OnCaptionHitTest)
    ON_MESSAGE(WM_NCMOUSEMOVE, &ThisClass::OnCaptionMouseMove)
    ON_MESSAGE(WM_NCMOUSELEAVE, &ThisClass::OnCaptionMouseLeave)
    ON_MESSAGE(WM_NCPAINT, &ThisClass::OnCaptionPaint)
    ON_MESSAGE(WM_NCACTIVATE, &ThisClass::OnCaptionPaint)
    ON_MESSAGE(WM_KEYDOWN, &ThisClass::OnPaneDragKey)
    ON_MESSAGE(WM_KEYUP, &ThisClass::OnPaneDragKey)
    ON_MESSAGE(WM_CAPTURECHANGED, &ThisClass::OnCaptureChanged)
    ON_MESSAGE(WM_CANCELMODE, &ThisClass::OnCancelPaneDrag)
    ON_REGISTERED_MESSAGE(impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA, &ThisClass::OnCheckEmptyState)
    OPENMFC_MAP_ENTRY(WM_NCLBUTTONDBLCLK, 0, 0, 0, AfxSig_l_w_l, &ThisClass::OnCaptionDoubleClick)
    {0, 0, 0, 0, AfxSig_end, nullptr}
};
}
}
const AFX_MSGMAP classCMultiPaneFrameWnd_msgmap = { gbm_CMultiPaneFrameWnd, multi_frame_map::entries };
const AFX_MSGMAP classCPaneDivider_msgmap = { gbm_CPaneDivider, divider_map::dividerEntries };
const AFX_MSGMAP classCPaneFrameWnd_msgmap = { gbm_CPaneFrameWnd, frame_map::paneFrameEntries };
const AFX_MSGMAP classCPaneTrackingWnd_msgmap = { gbm_CPaneTrackingWnd, g_emptyMsgEntries_Pane16Msgmap };
const AFX_MSGMAP classCSmartDockingGroupGuidesWnd_msgmap = { gbm_CSmartDockingGroupGuidesWnd, g_emptyMsgEntries_Pane16Msgmap };
} } }  // namespace openmfc::detail::pane16msgmap
