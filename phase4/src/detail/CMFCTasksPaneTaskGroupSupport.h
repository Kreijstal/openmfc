#pragma once
// Shared internals of the former global_cmfctaskspanetaskgroup.cpp translation unit.
// Definitions live in detail/CMFCTasksPaneTaskGroupSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {} } }
using namespace openmfc::detail::cmfctaskspanetaskgroup;
// OpenMFC — real implementation of CMFCTasksPaneTaskGroup
//
// class CMFCTasksPaneTaskGroup : public CObject   size(152):
//    0 | +--- (base CObject) {vfptr}
//    8 | m_pPage        (CMFCTasksPanePropertyPage*)
//   16 | m_strName      (CString — one heap pointer)
//   24 | m_lstTasks     (CObList — 56 bytes: vfptr + 4 ptrs + 2 INT_PTR)
//   80 | m_bIsBottom    (BOOL)
//   84 | m_bIsSpecial   (BOOL)
//   88 | m_bIsCollapsed (BOOL)
//   92 | m_rect         (CRect, 16)
//  108 | m_rectGroup    (CRect, 16)
//  124 | m_sizeIcon     (CSize, 8)
//  136 | m_hIcon        (HICON)
//  144 | m_clrText      (COLORREF)
//  148 | m_clrTextHot   (COLORREF)
//  (tail pad -> 152)
//
#include <windows.h>
#include <cstddef>
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

struct CRuntimeClass;
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);


namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
struct S_Cmfctaskspanetaskgroup {
    void*    vfptr;            //   0  CObject vtable
    void*    m_pPage;          //   8  CMFCTasksPanePropertyPage*
    void*    m_strName;        //  16  CString backing pointer
    char     m_lstTasks[56];   //  24  CObList (vfptr + 4 ptrs + 2 INT_PTR)
    int      m_bIsBottom;      //  80
    int      m_bIsSpecial;     //  84
    int      m_bIsCollapsed;   //  88
    RECT     m_rect;           //  92
    RECT     m_rectGroup;      // 108
    SIZE     m_sizeIcon;       // 124
    void*    m_hIcon;          // 136  HICON
    COLORREF m_clrText;        // 144
    COLORREF m_clrTextHot;     // 148
};
} } }


namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
struct OpenMfcRuntimeClass_Cmfctaskspanetaskgroup {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass_Cmfctaskspanetaskgroup* m_pBaseClass;
    void* m_pClassContext;
};
} } }

namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
extern OpenMfcRuntimeClass_Cmfctaskspanetaskgroup g_CMFCTasksPaneTaskGroup_rtti;
} } }

namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
OpenMfcRuntimeClass_Cmfctaskspanetaskgroup* GetBaseClass_Cmfctaskspanetaskgroup();
} } }


// Forward decl so the vtable can reference the exported thunk.
extern "C" int MS_ABI
impl__SetACCData_CMFCTasksPaneTaskGroup__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pParent, void* pData);


// --- CObject base-slot wrappers (no exports for these) --------------------

// GetRuntimeClass: no CRuntimeClass descriptor is authored for this helper
// class; install a local descriptor with CObject as base.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
const void* MS_ABI vslot_GetRuntimeClass_Cmfctaskspanetaskgroup(void* /*p*/);
} } }

// Serialize(CArchive&): CMFCTasksPaneTaskGroup is not serializable; the base
// CObject::Serialize is a no-op.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
void MS_ABI vslot_Serialize_Cmfctaskspanetaskgroup(void* pThis, void* pAr);
} } }

namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
void MS_ABI vslot_AssertValid_Cmfctaskspanetaskgroup(void* pThis);
} } }
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
void MS_ABI vslot_Dump_Cmfctaskspanetaskgroup(void* pThis, void* /*dc*/);
} } }

// Vector-deleting-destructor slot (CMFCTasksPaneTaskGroup::{dtor}).
// The real dtor empties m_lstTasks and clears m_pPage; the task list is owned
// through CObList internals not reconstructed here, so we conservatively drop
// the owning pointers and free storage when requested.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
void* MS_ABI vdtor_Cmfctaskspanetaskgroup(void* p, unsigned flags);
} } }

// Vtable in exact MSVC slot order.
namespace openmfc { namespace detail { namespace cmfctaskspanetaskgroup {
extern void* const g_CMFCTasksPaneTaskGroup_vtbl[6];
} } }


