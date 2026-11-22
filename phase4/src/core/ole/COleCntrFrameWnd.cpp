// The server uses these frame wrappers to manage its own bars on the container HWND.
#define OPENMFC_APPCORE_IMPL
#include "detail/COleCntrFrameWndSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include "detail/ClassicDockingSupport.h"
#include <new>

COleCntrFrameWnd::COleCntrFrameWnd(COleIPFrameWnd* frame) : m_pInPlaceFrame(frame) {}

COleCntrFrameWnd::~COleCntrFrameWnd() {
    DestroyControlBars();
    // This window belongs to the container and was never attached to our handle map.
    m_hWnd = nullptr;
}

void COleCntrFrameWnd::DestroyControlBars() {
    using namespace openmfc::detail::frame;
    auto& bars = ControlBars(this);
    while (!bars.IsEmpty()) {
        auto* bar = static_cast<CControlBar*>(bars.RemoveHead());
        if (!bar || bar->m_pDockSite != this) continue;
        bar->m_pDockSite = nullptr;
        bar->m_pInPlaceOwner = nullptr;
        if (::IsWindow(bar->m_hWnd)) bar->DestroyWindow();
    }
}

int COleCntrFrameWnd::OnCmdMsg(UINT id, int code, void* extra, AFX_CMDHANDLERINFO* handler) {
    return m_pInPlaceFrame && m_pInPlaceFrame->OnCmdMsg(id, code, extra, handler);
}

void COleCntrFrameWnd::RecalcLayout(BOOL) {
    using namespace openmfc::detail::dock;
    m_retailTail.m_dwBorderFlags &= ~(idleLayout | idleNotify);
    if (m_pInPlaceFrame) m_pInPlaceFrame->RecalcContainerControlBars(this);
}

void COleCntrFrameWnd::PostNcDestroy() {}

void COleCntrFrameWnd::OnIdleUpdateCmdUI() {
    if (m_retailTail.m_dwBorderFlags & (openmfc::detail::dock::idleLayout | openmfc::detail::dock::idleNotify))
        RecalcLayout(TRUE);
    if (m_pInPlaceFrame) impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(m_pInPlaceFrame);
}

// Symbol: ??0COleCntrFrameWnd@@QEAA@PEAVCOleIPFrameWnd@@@Z
extern "C" COleCntrFrameWnd* AFXAPI impl___0COleCntrFrameWnd__QEAA_PEAVCOleIPFrameWnd___Z(
    void* storage, COleIPFrameWnd* frame) {
    return storage ? ::new (storage) COleCntrFrameWnd(frame) : nullptr;
}

// Symbol: ??1COleCntrFrameWnd@@UEAA@XZ
extern "C" void AFXAPI impl___1COleCntrFrameWnd__UEAA_XZ(COleCntrFrameWnd* frame) {
    if (frame) frame->COleCntrFrameWnd::~COleCntrFrameWnd();
}

// Symbol: ?OnCmdMsg@COleCntrFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z
extern "C" int AFXAPI impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    COleCntrFrameWnd* frame, UINT id, int code, void* extra, AFX_CMDHANDLERINFO* handler) {
    return frame && frame->COleCntrFrameWnd::OnCmdMsg(id, code, extra, handler);
}

// Symbol: ?OnIdleUpdateCmdUI@COleCntrFrameWnd@@QEAAXXZ
extern "C" void AFXAPI impl__OnIdleUpdateCmdUI_COleCntrFrameWnd__QEAAXXZ(COleCntrFrameWnd* frame) {
    if (frame) frame->OnIdleUpdateCmdUI();
}

// Symbol: ?PostNcDestroy@COleCntrFrameWnd@@MEAAXXZ
extern "C" void AFXAPI impl__PostNcDestroy_COleCntrFrameWnd__MEAAXXZ(COleCntrFrameWnd* frame) {
    if (frame) frame->COleCntrFrameWnd::PostNcDestroy();
}

// Symbol: ?RecalcLayout@COleCntrFrameWnd@@UEAAXH@Z
extern "C" void AFXAPI impl__RecalcLayout_COleCntrFrameWnd__UEAAXH_Z(COleCntrFrameWnd* frame, BOOL notify) {
    if (frame) frame->COleCntrFrameWnd::RecalcLayout(notify);
}
