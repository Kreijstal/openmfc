// OpenMFC — real implementation of COleCntrFrameWnd
//
// COleCntrFrameWnd is the container-side frame window MFC creates around a
// server object that is being edited in place. It exists solely to forward
// layout recalculation and command routing to the *in-place* frame window
// (COleIPFrameWnd) supplied at construction, and to suppress the automatic
// "delete this" that CFrameWnd::PostNcDestroy performs (the container frame is
// an embedded member, not a heap object).
//
//   class COleCntrFrameWnd  size(480):
//     0   | +--- (base CFrameWnd : CWnd : CCmdTarget : CObject)
//     0   | | {vfptr}
//     8   | | m_dwRef ... (CFrameWnd body)
//   472   | m_pInPlaceFrame  (COleIPFrameWnd*)
//
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view of the object. Only the vfptr (offset 0) and the single
// derived member m_pInPlaceFrame (offset 472) are addressed here; the CFrameWnd
// body in between is opaque padding.
struct S {
    void* vfptr;               // 0
    char  _cframewnd[472 - 8]; // 8 .. 472  (CFrameWnd / CWnd / CCmdTarget body)
    void* m_pInPlaceFrame;     // 472  COleIPFrameWnd*
};

static_assert(sizeof(S) == 480, "COleCntrFrameWnd must be size 480");
static_assert(offsetof(S, m_pInPlaceFrame) == 472, "m_pInPlaceFrame @472");
static_assert(offsetof(S, vfptr) == 0, "vfptr @0");

// OnCmdMsg lives at vtable slot 5 for every CFrameWnd-derived type (see the
// harvested vtable), so we can route through the in-place frame's *actual*
// runtime type by dispatching that slot directly.
typedef int (MS_ABI *OnCmdMsgFn)(void*, unsigned int, int, void*, void*);
enum { VT_ONCMDMSG_SLOT = 5 };

} // namespace

// --- external impl_ thunks we delegate to (defined in sibling TUs) ----------
extern "C" int  MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__OnIdleUpdateCmdUI_COleIPFrameWnd__IEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(void* pThis);
extern "C" void MS_ABI impl__OnFinalRelease_CWnd__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_CFrameWnd__UEAAXH_Z(void* pThis, int bNotify);

// --- forward decls of our own exports referenced by the vtable --------------
extern "C" void MS_ABI impl___1COleCntrFrameWnd__UEAA_XZ(void* pThis);
extern "C" int  MS_ABI impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
extern "C" void MS_ABI impl__PostNcDestroy_COleCntrFrameWnd__MEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RecalcLayout_COleCntrFrameWnd__UEAAXH_Z(void* pThis, int bNotify);

namespace {

// --- vtable slot wrappers ---------------------------------------------------

// slot 0: CFrameWnd::GetRuntimeClass — COleCntrFrameWnd inherits the frame's
// runtime-class getter; delegate to it so RTTI queries resolve.
void* MS_ABI vt_GetRuntimeClass(void* pThis) {
    return impl__GetRuntimeClass_CFrameWnd__UEBAPEAUCRuntimeClass__XZ(pThis);
}

// slot 1: COleCntrFrameWnd::{dtor} — vector-deleting-destructor wrapper. Runs
// the real destructor, then frees storage when the delete flag (bit 0) is set.
void* MS_ABI vt_vdtor(void* p, unsigned int flags) {
    impl___1COleCntrFrameWnd__UEAA_XZ(p);
    if (flags & 1)
        ::operator delete(p);
    return p;
}

// slot 2: CObject::Serialize — CObject's implementation is a no-op.
void MS_ABI vt_Serialize(void* /*pThis*/, void* /*ar*/) {
}

// slot 3: CObject::AssertValid — no-op in release semantics.
void MS_ABI vt_AssertValid(void* /*pThis*/) {
}

// slot 4: CObject::Dump — no-op.
void MS_ABI vt_Dump(void* /*pThis*/, void* /*dc*/) {
}

// slot 5: COleCntrFrameWnd::OnCmdMsg — dispatch to our exported override.
int MS_ABI vt_OnCmdMsg(void* pThis, unsigned int nID, int nCode,
                       void* pExtra, void* pHandlerInfo) {
    return impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
        pThis, nID, nCode, pExtra, pHandlerInfo);
}

// slot 6: CWnd::OnFinalRelease — inherited; delegate to CWnd's implementation.
void MS_ABI vt_OnFinalRelease(void* pThis) {
    impl__OnFinalRelease_CWnd__UEAAXXZ(pThis);
}

// slot 7: CCmdTarget::IsInvokeAllowed — the base default permits every DISPID.
int MS_ABI vt_IsInvokeAllowed(void* /*pThis*/, unsigned int /*dispid*/) {
    return TRUE;
}

// slot 8: CCmdTarget::GetDispatchIID — no automation IID; base returns FALSE
// and leaves the caller's IID untouched.
int MS_ABI vt_GetDispatchIID(void* /*pThis*/, void* /*pIID*/) {
    return FALSE;
}

// slot 9: CCmdTarget::GetTypeInfoCount — no type information exposed: returns 0.
unsigned int MS_ABI vt_GetTypeInfoCount(void* /*pThis*/) {
    return 0;
}

void* const g_COleCntrFrameWnd_vtbl[10] = {
    (void*)&vt_GetRuntimeClass,  // 0  CFrameWnd::GetRuntimeClass
    (void*)&vt_vdtor,            // 1  COleCntrFrameWnd::{dtor}
    (void*)&vt_Serialize,        // 2  CObject::Serialize
    (void*)&vt_AssertValid,      // 3  CObject::AssertValid
    (void*)&vt_Dump,             // 4  CObject::Dump
    (void*)&vt_OnCmdMsg,         // 5  COleCntrFrameWnd::OnCmdMsg
    (void*)&vt_OnFinalRelease,   // 6  CWnd::OnFinalRelease
    (void*)&vt_IsInvokeAllowed,  // 7  CCmdTarget::IsInvokeAllowed
    (void*)&vt_GetDispatchIID,   // 8  CCmdTarget::GetDispatchIID
    (void*)&vt_GetTypeInfoCount, // 9  CCmdTarget::GetTypeInfoCount
};

} // namespace

// Symbol: ??0COleCntrFrameWnd@@QEAA@PEAVCOleIPFrameWnd@@@Z
// COleCntrFrameWnd::COleCntrFrameWnd(COleIPFrameWnd* pInPlaceFrame)
//   Constructs the container frame around the given in-place frame. The base
//   CFrameWnd sub-object is default-initialised (all handles/pointers null,
//   ref count zero), the vfptr is installed, and the in-place frame back
//   pointer is recorded for the forwarding methods below.
extern "C" void* MS_ABI impl___0COleCntrFrameWnd__QEAA_PEAVCOleIPFrameWnd___Z(
    void* pThis, void* pInPlaceFrame) {
    S* self = (S*)pThis;
    memset(self, 0, sizeof(S));
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
    S* self = (S*)pThis;
    self->vfptr = (void*)&g_COleCntrFrameWnd_vtbl[0];
}

// Symbol: ?OnCmdMsg@COleCntrFrameWnd@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
// BOOL COleCntrFrameWnd::OnCmdMsg(UINT nID,int nCode,void*,AFX_CMDHANDLERINFO*)
//   Command routing gives the in-place frame first crack (via its own virtual
//   OnCmdMsg, so a derived in-place frame's handlers participate); if it does
//   not handle the command, fall through to the standard CFrameWnd routing.
extern "C" int MS_ABI impl__OnCmdMsg_COleCntrFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo) {
    S* self = (S*)pThis;
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
    S* self = (S*)pThis;
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
    S* self = (S*)pThis;
    if (self->m_pInPlaceFrame != nullptr) {
        // COleIPFrameWnd does not override RecalcLayout, so it resolves to the
        // frame-window implementation; run it against the in-place frame.
        impl__RecalcLayout_CFrameWnd__UEAAXH_Z(self->m_pInPlaceFrame, bNotify);
    }
}
