// CMFCTabDropTarget — OpenMFC implementation.
// Sources: global_cmfctabdroptarget.cpp

#include "detail/CMFCTabDropTargetSupport.h"

// Symbol: ?OnDragEnter@CMFCTabDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" DWORD MS_ABI
impl__OnDragEnter_CMFCTabDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* /*pWnd*/, void* pDataObject, DWORD dwKeyState, unsigned long long point)
{
    S_Cmfctabdroptarget* self = static_cast<S_Cmfctabdroptarget*>(pThis);
    if (self->m_pOwner != nullptr) {
        void** vt = OwnerVtbl(self->m_pOwner);
        return reinterpret_cast<pfnDragEnterOver>(vt[kOwnerOnDragEnter])(
            self->m_pOwner, pDataObject, dwKeyState, point);
    }
    return DROPEFFECT_NONE;
}
// Symbol: ?OnDragOver@CMFCTabDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" DWORD MS_ABI
impl__OnDragOver_CMFCTabDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* /*pWnd*/, void* pDataObject, DWORD dwKeyState, unsigned long long point)
{
    S_Cmfctabdroptarget* self = static_cast<S_Cmfctabdroptarget*>(pThis);
    if (self->m_pOwner != nullptr) {
        void** vt = OwnerVtbl(self->m_pOwner);
        return reinterpret_cast<pfnDragEnterOver>(vt[kOwnerOnDragOver])(
            self->m_pOwner, pDataObject, dwKeyState, point);
    }
    return DROPEFFECT_NONE;
}
// Symbol: ?OnDragLeave@CMFCTabDropTarget@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI
impl__OnDragLeave_CMFCTabDropTarget__UEAAXPEAVCWnd___Z(void* pThis, void* /*pWnd*/)
{
    S_Cmfctabdroptarget* self = static_cast<S_Cmfctabdroptarget*>(pThis);
    if (self->m_pOwner != nullptr) {
        void** vt = OwnerVtbl(self->m_pOwner);
        reinterpret_cast<pfnDragLeave>(vt[kOwnerOnDragLeave])(self->m_pOwner);
    }
}
// Symbol: ?OnDropEx@CMFCTabDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KKVCPoint@@@Z
extern "C" DWORD MS_ABI
impl__OnDropEx_CMFCTabDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KKVCPoint___Z(
    void* pThis, void* /*pWnd*/, void* pDataObject, DWORD dropEffect,
    DWORD /*dropList*/, unsigned long long point)
{
    S_Cmfctabdroptarget* self = static_cast<S_Cmfctabdroptarget*>(pThis);
    if (self->m_pOwner != nullptr) {
        void** vt = OwnerVtbl(self->m_pOwner);
        int handled = reinterpret_cast<pfnOnDrop>(vt[kOwnerOnDrop])(
            self->m_pOwner, pDataObject, dropEffect, point);
        return handled ? dropEffect : DROPEFFECT_NONE;
    }
    return DROPEFFECT_NONE;
}
// Symbol: ?Register@CMFCTabDropTarget@@QEAAHPEAVCMFCBaseTabCtrl@@@Z
extern "C" int MS_ABI
impl__Register_CMFCTabDropTarget__QEAAHPEAVCMFCBaseTabCtrl___Z(void* pThis, void* pOwner)
{
    S_Cmfctabdroptarget* self = static_cast<S_Cmfctabdroptarget*>(pThis);

    // CMFCTabDropTarget::Register: remember the owner, then perform the base
    // COleDropTarget::Register(pOwner) — a CMFCBaseTabCtrl is a CWnd, whose
    // m_hWnd lives at offset 64. The registered IDropTarget is the object's
    // own nested part (m_xDropTarget at offset 96), which the base ctor wired
    // up to the real MFC XDropTarget vtable.
    self->m_pOwner = pOwner;
    if (pOwner == nullptr)
        return FALSE;

    HWND hWnd = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pOwner) + 64);
    if (hWnd == nullptr)
        return FALSE;

    if (self->m_hWnd != nullptr)        // already registered
        return FALSE;

    LPDROPTARGET pDropTarget =
        reinterpret_cast<LPDROPTARGET>(reinterpret_cast<char*>(self) + offsetof(S_Cmfctabdroptarget, m_xDropTarget));

    if (RegisterDragDrop(hWnd, pDropTarget) != S_OK)
        return FALSE;

    self->m_hWnd = hWnd;
    return TRUE;
}
