#pragma once
// Shared internals of the former global_cmfcacceleratorkey.cpp translation unit.
// Definitions live in detail/CMFCAcceleratorKeySupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {} } }
using namespace openmfc::detail::cmfcacceleratorkey;
// OpenMFC: CMFCAcceleratorKey  (public CObject)
//
// Layout (cl.exe /d1reportSingleClassLayout), size(16):
//   0  {vfptr}     (CObject vfptr)
//   8  m_lpAccel   (LPACCEL) -- borrowed pointer to a caller-owned ACCEL
//
// A thin wrapper around a single ACCEL entry that formats it into a human
// readable accelerator string such as "Ctrl+Shift+A".  The object does NOT own
// the ACCEL it points at, so the destructor frees nothing.  This class is not
// DECLARE_DYNAMIC: its vtable slot 0 (GetRuntimeClass), plus Serialize /
// AssertValid / Dump, are the inherited CObject implementations.

#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"
#include "openmfc/afxstr.h"   // ABI-compatible CStringW (single m_pszData pointer)

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
struct S_Cmfcacceleratorkey {
    void*   vfptr;      // 0
    LPACCEL m_lpAccel;  // 8
};
} } }


// ---- vtable slot helpers (non-exported virtuals) ----------------------------

// Slot 0: CObject::GetRuntimeClass -- delegate to the real CObject impl so the
// object reports RuntimeClass "CObject" (this class is not DECLARE_DYNAMIC).
extern "C" void* MS_ABI
impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void* MS_ABI slot_GetRuntimeClass_Cmfcacceleratorkey(const void* pThis);
} } }

// Slot 2: CObject::Serialize
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void MS_ABI slot_Serialize_Cmfcacceleratorkey(void* pThis, void* pAr);
} } }

// Slot 3: CObject::AssertValid
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void MS_ABI slot_AssertValid_Cmfcacceleratorkey(const void* pThis);
} } }

// Slot 4: CObject::Dump
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void MS_ABI slot_Dump_Cmfcacceleratorkey(const void* pThis, void* /*dc*/);
} } }

// ---- shared formatting helper -----------------------------------------------
//
// Appends the display name of one virtual key to `str`.  Mirrors the real
// CMFCAcceleratorKey::AddVirtKeyStr: derive the hardware scan code from the
// virtual key, set the extended-key bit for the navigation/edit cluster, ask
// the OS for the localized key name via GetKeyNameText, and append a trailing
// "+" separator unless this is the final (base) key.
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void AddVirtKeyStrImpl(CString& str, UINT uiVirtKey, BOOL bLast);
} } }


// -----------------------------------------------------------------------------
// Exported thunks
// -----------------------------------------------------------------------------

namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
extern void* const g_CMFCAcceleratorKey_vtbl[5];
} } }






// -----------------------------------------------------------------------------
// vtable
// -----------------------------------------------------------------------------


// Slot 1: CMFCAcceleratorKey::{vector deleting destructor}.
namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
void* MS_ABI slot_vecDelDtor_Cmfcacceleratorkey(void* pThis, unsigned flags);
} } }

namespace openmfc { namespace detail { namespace cmfcacceleratorkey {
extern void* const g_CMFCAcceleratorKey_vtbl[5];
} } }

