// COleCntrFrameWnd — OpenMFC implementation.
// Sources: global_colecntrframewnd.cpp

#include "detail/COleCntrFrameWndSupport.h"

// Symbol: ??0COleCntrFrameWnd@@QEAA@PEAVCOleIPFrameWnd@@@Z
// COleCntrFrameWnd::COleCntrFrameWnd(COleIPFrameWnd* pInPlaceFrame)
//   Constructs the container frame around the given in-place frame. The base
//   CFrameWnd sub-object is default-initialised (all handles/pointers null,
//   ref count zero), the vfptr is installed, and the in-place frame back
//   pointer is recorded for the forwarding methods below.
extern "C" void* MS_ABI impl___0COleCntrFrameWnd__QEAA_PEAVCOleIPFrameWnd___Z(
    void* pThis, void* pInPlaceFrame) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    memset(self, 0, sizeof(S_Colecntrframewnd));
    self->vfptr = (void*)&g_COleCntrFrameWnd_vtbl[0];
    self->m_pInPlaceFrame = pInPlaceFrame;
    return pThis;
}
// Symbol: ??1COleCntrFrameWnd@@UEAA@XZ
// COleCntrFrameWnd::~COleCntrFrameWnd()
//   The container frame owns no heap or GDI resources of its own (the in-place
//   frame it references is owned elsewhere), so destruction merely keeps the
//   vfptr valid for the base-class destructor chain.
extern "C" void MS_ABI impl___1COleCntrFrameWnd__UEAA_XZ(void* pThis) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    self->vfptr = (void*)&g_COleCntrFrameWnd_vtbl[0];
}
// Symbol: ?OnCmdMsg@COleCntrFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
// BOOL COleCntrFrameWnd::OnCmdMsg(UINT nID,int nCode,void*,AFX_CMDHANDLERINFO*)
//   Command routing gives the in-place frame first crack (via its own virtual
//   OnCmdMsg, so a derived in-place frame's handlers participate); if it does
//   not handle the command, fall through to the standard CFrameWnd routing.
extern "C" int MS_ABI impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    if (self->m_pInPlaceFrame != nullptr) {
        void** vt = *(void***)self->m_pInPlaceFrame;
        OnCmdMsgFn fn = (OnCmdMsgFn)vt[VT_ONCMDMSG_SLOT];
        if (fn(self->m_pInPlaceFrame, nID, nCode, pExtra, pHandlerInfo))
            return TRUE;
    }
    return impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
}
// Symbol: ?OnIdleUpdateCmdUI@COleCntrFrameWnd@@QEAAXXZ
// void COleCntrFrameWnd::OnIdleUpdateCmdUI()
//   Idle-time command-UI refresh is delegated to the in-place frame so its
//   toolbars/status bar update; the container frame has no UI of its own.
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleCntrFrameWnd__QEAAXXZ(void* pThis) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    if (self->m_pInPlaceFrame != nullptr)
        impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(self->m_pInPlaceFrame);
}
// Symbol: ?PostNcDestroy@COleCntrFrameWnd@@MEAAXXZ
// void COleCntrFrameWnd::PostNcDestroy()
//   Deliberately overrides CFrameWnd::PostNcDestroy to do NOTHING: the
//   container frame is an embedded sub-object of the server item, not a
//   heap allocation, so it must not "delete this".
extern "C" void MS_ABI impl__PostNcDestroy_COleCntrFrameWnd__MEAAXXZ(void* /*pThis*/) {
    // intentionally empty — the frame is not heap-owned
}
// Symbol: ?RecalcLayout@COleCntrFrameWnd@@UEAAXH@Z
// void COleCntrFrameWnd::RecalcLayout(BOOL bNotify)
//   Layout is owned by the in-place frame; forward the recalc (and its notify
//   flag) there. With no in-place frame there is nothing to lay out.
extern "C" void MS_ABI impl__RecalcLayout_COleCntrFrameWnd__UEAAXH_Z(void* pThis, int bNotify) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    if (self->m_pInPlaceFrame != nullptr) {
        // COleIPFrameWnd does not override RecalcLayout, so it resolves to the
        // frame-window implementation; run it against the in-place frame.
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(self->m_pInPlaceFrame, bNotify);
    }
}

// COleCntrFrameWnd's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace colecntrframewnd {
extern void* const g_COleCntrFrameWnd_vtbl[10] = {
    (void*)&vt_GetRuntimeClass_Colecntrframewnd,  // 0  CFrameWnd::GetRuntimeClass
    (void*)&vt_vdtor_Colecntrframewnd,            // 1  COleCntrFrameWnd::{dtor}
    (void*)&vt_Serialize_Colecntrframewnd,        // 2  CObject::Serialize
    (void*)&vt_AssertValid_Colecntrframewnd,      // 3  CObject::AssertValid
    (void*)&vt_Dump_Colecntrframewnd,             // 4  CObject::Dump
    (void*)&vt_OnCmdMsg,         // 5  COleCntrFrameWnd::OnCmdMsg
    (void*)&vt_OnFinalRelease,   // 6  CWnd::OnFinalRelease
    (void*)&vt_IsInvokeAllowed,  // 7  CCmdTarget::IsInvokeAllowed
    (void*)&vt_GetDispatchIID,   // 8  CCmdTarget::GetDispatchIID
    (void*)&vt_GetTypeInfoCount, // 9  CCmdTarget::GetTypeInfoCount
};
} } }
