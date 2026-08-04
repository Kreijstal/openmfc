#pragma once
// Shared internals of the former global_cmfctabdroptarget.cpp translation unit.
// Definitions live in detail/CMFCTabDropTargetSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctabdroptarget {} } }
using namespace openmfc::detail::cmfctabdroptarget;
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


namespace openmfc { namespace detail { namespace cmfctabdroptarget {
struct S_Cmfctabdroptarget {
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
} } }


// ---------------------------------------------------------------------------
// CMFCBaseTabCtrl vtable slot indices for its drop callbacks
// (cl.exe /d1reportSingleClassLayoutCMFCBaseTabCtrl, MFC 14.51):
//    173  OnDrop(COleDataObject*, DROPEFFECT, CPoint) -> BOOL
//    174  OnDragEnter(COleDataObject*, DWORD, CPoint) -> DROPEFFECT
//    175  OnDragLeave() -> void
//    176  OnDragOver(COleDataObject*, DWORD, CPoint) -> DROPEFFECT
// CPoint is an 8-byte POD passed by value in one integer register.
// ---------------------------------------------------------------------------
namespace openmfc { namespace detail { namespace cmfctabdroptarget {
enum {
    kOwnerOnDrop      = 173,
    kOwnerOnDragEnter = 174,
    kOwnerOnDragLeave = 175,
    kOwnerOnDragOver  = 176
};
} } }

namespace openmfc { namespace detail { namespace cmfctabdroptarget {
typedef DWORD (MS_ABI *pfnDragEnterOver)(void* owner, void* pDataObject, DWORD dwKeyState, unsigned long long pt);
} } }
namespace openmfc { namespace detail { namespace cmfctabdroptarget {
typedef void  (MS_ABI *pfnDragLeave)(void* owner);
} } }
namespace openmfc { namespace detail { namespace cmfctabdroptarget {
typedef int   (MS_ABI *pfnOnDrop)(void* owner, void* pDataObject, DWORD dropEffect, unsigned long long pt);
} } }

namespace openmfc { namespace detail { namespace cmfctabdroptarget {
void** OwnerVtbl(void* owner);
} } }






