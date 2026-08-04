#pragma once
// Shared internals of the former global_cmfcribbonquickaccesstoolbardefaultstate.cpp translation unit.
// Definitions live in detail/CMFCRibbonQuickAccessToolBarDefaultStateSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {} } }
using namespace openmfc::detail::cmfcribbonquickaccesstoolbardefaultstate;
// OpenMFC real implementation of CMFCRibbonQuickAccessToolBarDefaultState
//
// Harvested layout (cl.exe /d1reportSingleClassLayout):
//   class CMFCRibbonQuickAccessToolBarDefaultState size(80):
//     +0  CArray<UINT,UINT>  m_arCommands
//     +40 CArray<BOOL,BOOL>  m_arVisibleState
//
// The class itself is NOT polymorphic (no vfptr at offset 0). Its two members
// are MFC CArray objects. A CArray derives from CObject, so each is:
//     +0  vfptr        (CObject vtable pointer; MFC-internal, left null here)
//     +8  m_pData      (TYPE*)
//     +16 m_nSize      (INT_PTR)
//     +24 m_nMaxSize   (INT_PTR)
//     +32 m_nGrowBy    (INT_PTR)
//   => sizeof == 40, two of them == 80.
//
// Both element types (UINT and BOOL) are 4 bytes, so a single set of
// element-size-parameterised helpers faithfully reproduces CArray semantics
// (SetSize grow policy, Add, Copy, RemoveAll).

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
struct CArrayS {
    void*     vfptr;      // 0  CObject vtable pointer
    void*     m_pData;    // 8
    long long m_nSize;    // 16
    long long m_nMaxSize; // 24
    long long m_nGrowBy;  // 32
};
} } }

namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
struct S_Cmfcribbonquickaccesstoolbardefaultstate {
    CArrayS m_arCommands;     // 0
    CArrayS m_arVisibleState; // 40
};
} } }


// Faithful reproduction of MFC CArray::SetSize(nNewSize, /*nGrowBy=*/-1).
namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
void arr_SetSize(CArrayS* a, long long nNewSize, size_t elem);
} } }

// CArray::Add for a 4-byte element type.
namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
void arr_Add4(CArrayS* a, unsigned val);
} } }

// CArray::Copy (elements are trivially copyable POD).
namespace openmfc { namespace detail { namespace cmfcribbonquickaccesstoolbardefaultstate {
void arr_Copy(CArrayS* dst, const CArrayS* src, size_t elem);
} } }





