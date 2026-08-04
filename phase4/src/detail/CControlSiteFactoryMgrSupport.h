#pragma once
// Shared internals of the former global_ccontrolsitefactorymgr.cpp translation unit.
// Definitions live in detail/CControlSiteFactoryMgrSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {} } }
using namespace openmfc::detail::ccontrolsitefactorymgr;
// OpenMFC real implementation: CControlSiteFactoryMgr
//
// CControlSiteFactoryMgr manages a list of IControlSiteFactory* objects and
// dispatches control-site creation to them (falling back to a default OLE
// control-site factory).  It derives from CNoTrackObject (single vfptr) and
// owns a CList<IControlSiteFactory*,IControlSiteFactory*> plus a default
// factory pointer.
//
// Harvested layout (size 72):
//   0  : {vfptr}   (from CNoTrackObject)
//   8  : m_lstFactory  (CList, 56 bytes)
//   64 : m_pOleControlSiteDefaultFactory  (COleControlSiteFactory*)

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


// One node of an MFC CList<TYPE,ARG_TYPE>: { CNode* pNext; CNode* pPrev; TYPE data; }
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
struct CNode {
    CNode* pNext;
    CNode* pPrev;
    void*  data;   // IControlSiteFactory*
};
} } }

// Layout-faithful mirror of MFC CList<TYPE,ARG_TYPE> (derives from CObject).
//   0  : {vfptr}
//   8  : m_pNodeHead
//   16 : m_pNodeTail
//   24 : m_nCount     (INT_PTR)
//   32 : m_pNodeFree
//   40 : m_pBlocks    (CPlex*)
//   48 : m_nBlockSize (INT_PTR)
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
struct CListMirror {
    void*      vfptr;
    CNode*     m_pNodeHead;
    CNode*     m_pNodeTail;
    long long  m_nCount;
    CNode*     m_pNodeFree;
    void*      m_pBlocks;
    long long  m_nBlockSize;
};
} } }

namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
struct S_Ccontrolsitefactorymgr {
    void*        vfptr;                             // 0
    CListMirror  m_lstFactory;                      // 8
    void*        m_pOleControlSiteDefaultFactory;   // 64
};
} } }


// Vtable slot signature for the single pure-virtual IControlSiteFactory method:
//   virtual COleControlSite* CreateSite(COleControlContainer*, const CControlCreationInfo&);
// The factory objects are real (MSVC-compiled), so dispatch through their
// vtable with the MS x64 ABI.  const-ref arrives as a pointer.
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
typedef void* (MS_ABI* PFN_FactoryCreateSite)(void* self, void* pCtrlCont, const void* creationInfo);
} } }

// --- CList helpers (faithful AddTail / Find / RemoveAt semantics) ----------

namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
void list_add_tail(CListMirror* pl, void* value);
} } }

// Remove the first node whose data equals value; returns TRUE if removed.
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
BOOL list_remove_value(CListMirror* pl, void* value);
} } }

namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
void list_destroy(CListMirror* pl);
} } }


// Forward declaration: the real (non-deleting) destructor thunk.
extern "C" void MS_ABI impl___1CControlSiteFactoryMgr__UEAA_XZ(void* pThis);


// Single vtable slot: the vector-deleting destructor (MSVC folds ~dtor here).
namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
void* MS_ABI vdtor_Ccontrolsitefactorymgr(void* p, unsigned flags);
} } }

namespace openmfc { namespace detail { namespace ccontrolsitefactorymgr {
extern void* const g_CControlSiteFactoryMgr_vtbl[1];
} } }






