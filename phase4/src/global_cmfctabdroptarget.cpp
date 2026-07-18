// OpenMFC — CMFCTabDropTarget real implementation
//
// CMFCTabDropTarget : public COleDropTarget
// The drop target embedded in every CMFCBaseTabCtrl. It simply forwards the
// OLE drop-target callbacks to its owning tab control (m_pOwner), whose own
// virtual OnDragEnter/OnDragOver/OnDragLeave/OnDrop methods implement the tab
// drag-reorder logic.
//
// Real MSVC layout (cl.exe /d1reportSingleClassLayout, MFC 14.51, size(112)):
//    0  {vfptr}                     (CObject)
//    8  m_dwRef                     (CCmdTarget)
//   16  m_pOuterUnknown
//   24  m_xInnerUnknown  (nested)
//   32  m_xDispatch      (nested)
//   40  m_bResultExpected
//   48  m_xConnPtContainer (nested)
//   56  m_pModuleState
//   64  m_hWnd                      (COleDropTarget: registered window)
//   72  m_lpDataObject
//   80  m_nTimerID
//   84  m_dwLastTick
//   88  m_nScrollDelay
//   96  m_xDropTarget    (nested IDropTarget)
//  104  m_pOwner                    (CMFCTabDropTarget: CMFCBaseTabCtrl*)

#include <windows.h>
#include <ole2.h>
#include <oleidl.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct S {
    void*        vfptr;              //  0
    unsigned int m_dwRef;           //  8
    unsigned int _pad0;             // 12
    void*        m_pOuterUnknown;   // 16
    void*        m_xInnerUnknown;   // 24
    void*        m_xDispatch;       // 32
    int          m_bResultExpected; // 40
    unsigned int _pad1;             // 44
    void*        m_xConnPtContainer;// 48
    void*        m_pModuleState;    // 56
    void*        m_hWnd;            // 64  registered HWND
    void*        m_lpDataObject;    // 72
    unsigned int m_nTimerID;        // 80
    unsigned int m_dwLastTick;      // 84
    unsigned int m_nScrollDelay;    // 88
    unsigned int _pad2;             // 92
    void*        m_xDropTarget;     // 96  nested IDropTarget (vfptr)
    void*        m_pOwner;          //104  CMFCBaseTabCtrl*
};

static_assert(sizeof(S) == 112,                     "CMFCTabDropTarget size");
static_assert(offsetof(S, m_dwRef) == 8,            "m_dwRef offset");
static_assert(offsetof(S, m_hWnd) == 64,            "m_hWnd offset");
static_assert(offsetof(S, m_xDropTarget) == 96,     "m_xDropTarget offset");
static_assert(offsetof(S, m_pOwner) == 104,         "m_pOwner offset");

// ---------------------------------------------------------------------------
// CMFCBaseTabCtrl vtable slot indices for its drop callbacks
// (cl.exe /d1reportSingleClassLayoutCMFCBaseTabCtrl, MFC 14.51):
//    173  OnDrop(COleDataObject*, DROPEFFECT, CPoint) -> BOOL
//    174  OnDragEnter(COleDataObject*, DWORD, CPoint) -> DROPEFFECT
//    175  OnDragLeave() -> void
//    176  OnDragOver(COleDataObject*, DWORD, CPoint) -> DROPEFFECT
// CPoint is an 8-byte POD passed by value in one integer register.
// ---------------------------------------------------------------------------
enum {
    kOwnerOnDrop      = 173,
    kOwnerOnDragEnter = 174,
    kOwnerOnDragLeave = 175,
    kOwnerOnDragOver  = 176
};

typedef DWORD (MS_ABI *pfnDragEnterOver)(void* owner, void* pDataObject, DWORD dwKeyState, unsigned long long pt);
typedef void  (MS_ABI *pfnDragLeave)(void* owner);
typedef int   (MS_ABI *pfnOnDrop)(void* owner, void* pDataObject, DWORD dropEffect, unsigned long long pt);

static inline void** OwnerVtbl(void* owner) { return *reinterpret_cast<void***>(owner); }

} // namespace

// Symbol: ?OnDragEnter@CMFCTabDropTarget@@UEAAKPEAVCWnd@@PEAVCOleDataObject@@KVCPoint@@@Z
extern "C" DWORD MS_ABI
impl__OnDragEnter_CMFCTabDropTarget__UEAAKPEAVCWnd__PEAVCOleDataObject__KVCPoint___Z(
    void* pThis, void* /*pWnd*/, void* pDataObject, DWORD dwKeyState, unsigned long long point)
{
    S* self = static_cast<S*>(pThis);
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
    S* self = static_cast<S*>(pThis);
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
    S* self = static_cast<S*>(pThis);
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
    S* self = static_cast<S*>(pThis);
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
    S* self = static_cast<S*>(pThis);

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
        reinterpret_cast<LPDROPTARGET>(reinterpret_cast<char*>(self) + offsetof(S, m_xDropTarget));

    if (RegisterDragDrop(hWnd, pDropTarget) != S_OK)
        return FALSE;

    self->m_hWnd = hWnd;
    return TRUE;
}
