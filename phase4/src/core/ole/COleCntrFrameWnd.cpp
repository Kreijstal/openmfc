// COleCntrFrameWnd — OpenMFC implementation.
// Sources: global_colecntrframewnd.cpp

#include "detail/COleCntrFrameWndSupport.h"

// Symbol: ??0COleCntrFrameWnd@@QEAA@PEAVCOleIPFrameWnd@@@Z
// COleCntrFrameWnd::COleCntrFrameWnd(COleIPFrameWnd* pInPlaceFrame)
//   OpenMFC approximation: zeroes the shadow object, installs this unit's
//   partial dispatch table, and records the in-place frame pointer. Retail
//   construction calls the CFrameWnd constructor and an additional helper;
//   this memset is not equivalent to those base-class initializers.
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
//   OpenMFC approximation: this body only restores the derived dispatch-table
//   pointer. Retail performs additional frame/handle cleanup and calls the
//   CFrameWnd destructor; that cleanup is not reproduced here.
extern "C" void MS_ABI impl___1COleCntrFrameWnd__UEAA_XZ(void* pThis) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    self->vfptr = (void*)&g_COleCntrFrameWnd_vtbl[0];
}
// Symbol: ?OnCmdMsg@COleCntrFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
// BOOL COleCntrFrameWnd::OnCmdMsg(UINT nID,int nCode,void*,AFX_CMDHANDLERINFO*)
//   OpenMFC approximation: dispatches to the in-place frame's slot 5, then
//   falls back to CFrameWnd if unhandled. Retail temporarily updates and
//   restores AFX thread command-routing state and dispatches to the in-place
//   frame; it does not take this fallback path.
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
//   OpenMFC approximation: forwards to the in-place frame's idle-update thunk.
//   Retail checks frame flags, conditionally invokes a virtual on this frame,
//   then walks the parent chain and sends WM_IDLEUPDATECMDUI (0x363); this
//   forwarding body does not reproduce those steps.
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleCntrFrameWnd__QEAAXXZ(void* pThis) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    if (self->m_pInPlaceFrame != nullptr)
        impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(self->m_pInPlaceFrame);
}
// Symbol: ?PostNcDestroy@COleCntrFrameWnd@@MEAAXXZ
// void COleCntrFrameWnd::PostNcDestroy()
// Stub: this export did not resolve in either retail image, so its body and
// whether it must suppress CFrameWnd cleanup remain unverified.
extern "C" void MS_ABI impl__PostNcDestroy_COleCntrFrameWnd__MEAAXXZ(void* /*pThis*/) {
    // Retail implementation unavailable; do not infer its behavior.
}
// Symbol: ?RecalcLayout@COleCntrFrameWnd@@UEAAXH@Z
// void COleCntrFrameWnd::RecalcLayout(BOOL bNotify)
//   OpenMFC approximation: forwards to CFrameWnd::RecalcLayout on the
//   in-place frame. Retail has a reentrancy guard and flag handling, and makes
//   conditional virtual callbacks based on the in-place/active-document state;
//   this direct forward is not equivalent.
extern "C" void MS_ABI impl__RecalcLayout_COleCntrFrameWnd__UEAAXH_Z(void* pThis, int bNotify) {
    S_Colecntrframewnd* self = (S_Colecntrframewnd*)pThis;
    if (self->m_pInPlaceFrame != nullptr) {
        // COleIPFrameWnd has its own RecalcLayout override; this direct base
        // thunk intentionally bypasses it and is only an OpenMFC approximation.
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(self->m_pInPlaceFrame, bNotify);
    }
}

// Partial dispatch table for COleCntrFrameWnd's OpenMFC implementation,
// defined beside its targets. It contains only slots 0-9, not the complete
// CFrameWnd MSVC vtable (which has virtuals at slots such as 96); calls through
// omitted slots are unsupported and would read beyond this array.
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
