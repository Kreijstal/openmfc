#pragma once
// Shared internals of the former global_cribboncategoryscroll.cpp translation unit.
// Definitions live in detail/CRibbonCategoryScrollSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {} } }
using namespace openmfc::detail::cribboncategoryscroll;
// OpenMFC: CRibbonCategoryScroll  (private CMFCRibbonButton : CMFCRibbonBaseElement
//   : CMFCBaseAccessibleObject : CCmdTarget : CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(632):
//   0    {vfptr}     (CObject vfptr, shared through the whole single-inheritance chain)
//   8    m_dwRef     (CCmdTarget::m_dwRef) ... plus the full CMFCRibbonButton state
//   624  m_bIsLeft   (BOOL) -- the sole member CRibbonCategoryScroll adds
//
// This is the internal left/right scroll button placed in a ribbon category's
// tab strip when the tabs overflow.  m_bIsLeft selects the scroll direction.
// It is created by CMFCRibbonCategory (declared a friend) which then sets
// m_bIsLeft directly, so the default ctor simply leaves it FALSE.

#include <windows.h>
#include <cstddef>
#include <cstring>
#include "openmfc/afx.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

struct CRuntimeClass;

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCRibbonButton__UEBAPEAUCRuntimeClass__XZ(const void* pThis);


namespace openmfc { namespace detail { namespace cribboncategoryscroll {
struct S_Cribboncategoryscroll {
    void* vfptr;         // 0
    char  pad[616];      // 8 .. 623   (CCmdTarget/CMFCRibbonBaseElement/CMFCRibbonButton state)
    int   m_bIsLeft;     // 624        BOOL
    char  tail[4];       // 628 .. 631 alignment padding
};
} } }


namespace openmfc { namespace detail { namespace cribboncategoryscroll {
struct OpenMfcRuntimeClass_Cribboncategoryscroll {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass_Cribboncategoryscroll* m_pBaseClass;
    void* m_pClassContext;
};
} } }

namespace openmfc { namespace detail { namespace cribboncategoryscroll {
extern OpenMfcRuntimeClass_Cribboncategoryscroll g_CRibbonCategoryScroll_rtti;
} } }

namespace openmfc { namespace detail { namespace cribboncategoryscroll {
OpenMfcRuntimeClass_Cribboncategoryscroll* GetBaseClass_Cribboncategoryscroll();
} } }

// Non-zero default field values of the CMFCRibbonButton / CMFCRibbonBaseElement
// base, harvested from the real mfc140u layout (cl.exe) and verified byte-exact
// against the real DLL's default construction (differential dump 2026-07-18).
// Offset -> int value. Every other scalar field defaults to 0. Pointer members
// (CString buffers, embedded CArray/XDispatch vtables, HMENU/HICON handles) are
// left null here — they legitimately differ from the real DLL's addresses.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
inline void InstallBaseDefaults(void* p) {
    struct FV { int off; int val; };
    static const FV kDefaults[] = {
        {   8, 1 },   // m_dwRef (CCmdTarget reference count defaults to 1)
        {  40, 1 },   // m_bResultExpected
        { 284, -1 },  // m_nRow
        { 348, 1 },   // m_bIsVisible
        { 360, 1 },   // m_bDrawDefaultIcon
        { 372, 1 },   // m_bEnableUpdateTooltipInfo
        { 376, 1 },   // m_bEnableTooltipInfoShortcut
        { 400, 3 },   // m_szMargin.cx
        { 404, 3 },   // m_szMargin.cy
        { 440, -1 },  // m_nSmallImageIndex
        { 444, -1 },  // m_nLargeImageIndex
        { 448, 2 },   // m_nMenuArrowMargin
        { 456, -1 },  // m_nWrapIndex
        { 492, 1 },   // m_bIsDefaultCommand
    };
    for (const FV& f : kDefaults)
        *reinterpret_cast<int*>(static_cast<char*>(p) + f.off) = f.val;
}
} } }

// ---- vtable slot helpers (non-exported virtuals) ---------------------------

// Slot 0: CMFCRibbonButton::GetRuntimeClass (const).  Runtime-class metadata for
// this internal type is not part of the public surface; return a local descriptor
// derived from CMFCRibbonButton.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void* MS_ABI slot_GetRuntimeClass_Cribboncategoryscroll(const void* /*pThis*/);
} } }

// Slot 2: CObject::Serialize -- base implementation is a no-op.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void MS_ABI slot_Serialize_Cribboncategoryscroll(void* pThis, void* pAr);
} } }

// Slot 3: CObject::AssertValid (const) -- no-op in release.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void MS_ABI slot_AssertValid_Cribboncategoryscroll(const void* pThis);
} } }

// Slot 4: CObject::Dump (const) -- no-op in release.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void MS_ABI slot_Dump_Cribboncategoryscroll(const void* pThis, void* /*dc*/);
} } }

// Slot 5: CCmdTarget::OnCmdMsg -- base behavior, conservative no-op for unknown
// commands when there is no object state.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
int MS_ABI slot_OnCmdMsg(void* pThis, unsigned nID, int nCode,
                         void* pExtra, void* pHandlerInfo);
} } }

// Slot 6: CCmdTarget::OnFinalRelease -- default just self-deletes; the ribbon
// element is not COM-owned here, so no-op.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void MS_ABI slot_OnFinalRelease(void* pThis);
} } }

// Slot 7: CCmdTarget::IsInvokeAllowed -- automation gate, allow by default.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
int MS_ABI slot_IsInvokeAllowed(void* pThis, unsigned /*dispid*/);
} } }

// Slot 8: CCmdTarget::GetDispatchIID -- no dispinterface.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
int MS_ABI slot_GetDispatchIID(void* pThis, void* /*pIID*/);
} } }

// Slot 9: CCmdTarget::GetTypeInfoCount -- no type info.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
unsigned MS_ABI slot_GetTypeInfoCount(void* pThis);
} } }

extern void* const g_CRibbonCategoryScroll_vtbl[10];


// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------






// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------


// Slot 1: CRibbonCategoryScroll::{vector deleting destructor}.  No owned
// resources; release the object when the low flag bit is set.
namespace openmfc { namespace detail { namespace cribboncategoryscroll {
void* MS_ABI slot_vecDelDtor_Cribboncategoryscroll(void* pThis, unsigned flags);
} } }

extern void* const g_CRibbonCategoryScroll_vtbl[10];

