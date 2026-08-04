#pragma once
// Shared internals of the former cobject_impl.cpp translation unit.
// Definitions live in detail/CRuntimeClassSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cruntimeclass {} } }
using namespace openmfc::detail::cruntimeclass;
// CObject and CRuntimeClass Implementation for OpenMFC
//
// Implements the root MFC class CObject and its runtime type information.
// This provides the foundation for MFC's RTTI system (separate from C++ RTTI).
//
// Key exports:
// - CObject::GetRuntimeClass() - virtual method returning CRuntimeClass*
// - CObject::GetThisClass() - static method returning CRuntimeClass* for CObject
// - CObject::IsKindOf() - type checking using CRuntimeClass hierarchy
// - CObject::IsSerializable() - check if class supports serialization
// - CRuntimeClass methods for dynamic object creation and type lookup

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include <windows.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cwchar>
#include "openmfc/afxwin.h"

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CObject::classCObject - MSVC symbol alias
// The inline static member is defined in afx.h with GCC mangling
#ifdef __clang__
extern "C" __attribute__((weak)) CRuntimeClass _ZN7CObject12classCObjectE = {
    "CObject",
    sizeof(CObject),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};
#endif

#ifdef __GNUC__
asm(".globl \"?classCObject@CObject@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCObject@CObject@@2UCRuntimeClass@@A\", _ZN7CObject12classCObjectE\n");
#endif

// =============================================================================
// CRuntimeClass Structure (must match MSVC MFC layout exactly)
// =============================================================================

// Forward declarations
// struct CRuntimeClass; // Defined in afxwin.h
// struct CObject;       // Defined in afxwin.h
// struct CArchive;      // Defined in afxwin.h

// Function pointer type for object creation
typedef CObject* (__cdecl *PFNCREATEOBJECT)();

// =============================================================================
// Global CRuntimeClass Registry (for FromName lookups)
// =============================================================================
// Note: Real MFC doesn't have m_pNextClass in CRuntimeClass struct.
// We use a separate array to track registered classes.

#include <vector>
namespace openmfc { namespace detail { namespace cruntimeclass {
std::vector<CRuntimeClass*>& GetClassRegistry();
} } }

namespace openmfc { namespace detail { namespace cruntimeclass {
CRuntimeClass*& GetClassRegistryHead();
} } }

// Register a class in the global list
namespace openmfc { namespace detail { namespace cruntimeclass {
void RegisterRuntimeClass(CRuntimeClass* pClass);
} } }

// Find a runtime class by name
namespace openmfc { namespace detail { namespace cruntimeclass {
CRuntimeClass* FindRuntimeClass(const char* lpszClassName);
} } }

// Initialize on first use
namespace openmfc { namespace detail { namespace cruntimeclass {
extern bool g_classesInitialized;
} } }
namespace openmfc { namespace detail { namespace cruntimeclass {
void InitializeClasses();
} } }

namespace openmfc { namespace detail { namespace cruntimeclass {
const CRuntimeClass* GetObjectRuntimeClass(const CObject* pObject);
} } }

// =============================================================================
// CObject Methods
// =============================================================================





// CObject::Serialize() - virtual member function
// Base implementation does nothing
namespace openmfc { namespace detail { namespace cruntimeclass {
extern "C" void MS_ABI impl__Serialize_CObject__UEAAXAEAVCArchive___Z(
    CObject* pThis,      // RCX = this pointer
    CArchive* ar         // RDX = archive
);
} } }

// C++ implementation of Serialize is already provided inline in afx.h
// The stub above handles the ABI export with mangled name

// =============================================================================
// CRuntimeClass Methods
// =============================================================================









// =============================================================================
// Helper Functions
// =============================================================================

// AfxClassInit - called during static initialization to register classes
namespace openmfc { namespace detail { namespace cruntimeclass {
extern "C" void MS_ABI impl__AfxClassInit__YAXPEAUCRuntimeClass___Z(
    CRuntimeClass* pNewClass  // RCX = class to register
);
} } }

// AfxDynamicDownCast - dynamic_cast equivalent for MFC
// Note: This is already in the mapping but we implement it here for completeness
namespace openmfc { namespace detail { namespace cruntimeclass {
extern "C" CObject* MS_ABI impl__AfxDynamicDownCast__YAPEAVCObject__PEAUCRuntimeClass__PEAV1__Z(
    CRuntimeClass* pClass,  // RCX = target class
    CObject* pObject        // RDX = object to cast
);
} } }
