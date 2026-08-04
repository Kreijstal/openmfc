#pragma once
// Shared internals of the former global_crecentpanecontainerinfo.cpp translation unit.
// Definitions live in detail/CRecentPaneContainerInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {} } }
using namespace openmfc::detail::crecentpanecontainerinfo;
// OpenMFC real implementation of CRecentPaneContainerInfo (mfc140u).
// Layout-faithful; MS_ABI thunks aliased from the .def as impl_ symbols.
#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// --------------------------------------------------------------------------
// Layout-faithful mirror. Harvested size(104), CObject base (vfptr @0).
//   0   {vfptr}
//   8   CRect m_rectDockedRect            (RECT, 16 bytes)
//  24   int   m_nRecentPercent
//  28   BOOL  m_bIsRecentLeftBar
//  32   CList<HWND,HWND> m_lstRecentListOfBars  (56 bytes, own CObject base)
//  88   CPaneContainer* m_pRecentBarContainer
//  96   CPaneContainer* m_pRecentContainerOfTabWnd
// --------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
struct CNode {
    CNode* pNext;
    CNode* pPrev;
    HWND   data;
};
} } }

// Mirror of CList<HWND,HWND> (derives from CObject).
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
struct ListS {
    void*   vfptr;        // +0  CObject vfptr
    CNode*  pNodeHead;    // +8
    CNode*  pNodeTail;    // +16
    INT_PTR nCount;       // +24
    CNode*  pNodeFree;    // +32
    void*   pBlocks;      // +40  CPlex*
    int     nBlockSize;   // +48
};
} } }

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
struct S_Crecentpanecontainerinfo {
    void*   vfptr;                       // 0
    RECT    m_rectDockedRect;            // 8
    int     m_nRecentPercent;           // 24
    int     m_bIsRecentLeftBar;         // 28  (BOOL)
    ListS   m_lstRecentListOfBars;      // 32
    void*   m_pRecentBarContainer;      // 88
    void*   m_pRecentContainerOfTabWnd; // 96
};
} } }


// ----- minimal but faithful CList helpers (nodes we own end-to-end) --------
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void list_removeall(S_Crecentpanecontainerinfo* s);
} } }

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void list_addtail(S_Crecentpanecontainerinfo* s, HWND h);
} } }

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void list_init_empty(S_Crecentpanecontainerinfo* s);
} } }

// Copy all persistent state from src into dst (shared by operator= / SetInfo).
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void copy_from(S_Crecentpanecontainerinfo* dst, S_Crecentpanecontainerinfo* src);
} } }


// forward decls of exported thunks used by the vtable
extern "C" void  MS_ABI impl___1CRecentPaneContainerInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__SetInfo_CRecentPaneContainerInfo__UEAAXAEAV1__Z(void* pThis, void* pSrc);
extern "C" void  MS_ABI impl__StoreDockInfo_CRecentPaneContainerInfo__UEAAXPEAVCPaneContainer__PEAVCDockablePane__1_Z(
    void* pThis, void* pRecentContainer, void* pBar, void* pTabbedBar);

// --------------------------------------------------------------------------
// vtable (exact harvested slot order)
//   0 CObject::GetRuntimeClass
//   1 {deleting dtor}
//   2 CObject::Serialize
//   3 CObject::AssertValid
//   4 CObject::Dump
//   5 CRecentPaneContainerInfo::StoreDockInfo
//   6 CRecentPaneContainerInfo::SetInfo
// --------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void* MS_ABI vslot_GetRuntimeClass_Crecentpanecontainerinfo(void* /*pThis*/);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void* MS_ABI vslot_deleting_dtor(void* pThis, unsigned flags);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void MS_ABI vslot_Serialize_Crecentpanecontainerinfo(void* pThis, void* pAr);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void MS_ABI vslot_AssertValid_Crecentpanecontainerinfo(void* pThis);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void MS_ABI vslot_Dump_Crecentpanecontainerinfo(void* pThis, void* /*pDC*/);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void MS_ABI vslot_StoreDockInfo(void* pThis, void* a, void* b, void* c);
} } }
namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
void MS_ABI vslot_SetInfo(void* pThis, void* pSrc);
} } }

namespace openmfc { namespace detail { namespace crecentpanecontainerinfo {
extern void* const g_CRecentPaneContainerInfo_vtbl[7];
} } }


// --------------------------------------------------------------------------
// exported thunks
// --------------------------------------------------------------------------






