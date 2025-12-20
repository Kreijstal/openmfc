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
#include "openmfc/afxwin.h"

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CObject::classCObject - MSVC symbol alias
// The inline static member is defined in afx.h with GCC mangling
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
static std::vector<CRuntimeClass*>& GetClassRegistry() {
    static std::vector<CRuntimeClass*> registry;
    return registry;
}

// Register a class in the global list
static void RegisterRuntimeClass(CRuntimeClass* pClass) {
    if (pClass) {
        auto& registry = GetClassRegistry();
        // Avoid duplicates
        for (auto* c : registry) {
            if (c == pClass) return;
        }
        registry.push_back(pClass);
    }
}

// Initialize on first use
static bool g_classesInitialized = false;
static void InitializeClasses() {
    if (g_classesInitialized) return;
    RegisterRuntimeClass(&CObject::classCObject);
    g_classesInitialized = true;
}

// =============================================================================
// CObject Methods
// =============================================================================

// CObject::GetRuntimeClass() - virtual method
// Symbol: ?GetRuntimeClass@CObject@@UBAPAUCRuntimeClass@@XZ
// Returns pointer to this object's CRuntimeClass
// Note: This is virtual, so derived classes override it
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(
    const CObject* pThis  // RCX = this pointer
) {
    (void)pThis;  // For CObject base, we return classCObject
    InitializeClasses();
    return &CObject::classCObject;
}

// CObject::GetThisClass() - static method
// Symbol: ?GetThisClass@CObject@@SAPAUCRuntimeClass@@XZ
// Returns pointer to CObject's CRuntimeClass (not virtual dispatch)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CObject__SAPEAUCRuntimeClass__XZ() {
    InitializeClasses();
    return &CObject::classCObject;
}

// CObject::IsKindOf() - const member function
// Symbol: ?IsKindOf@CObject@@QBAHPBUCRuntimeClass@@@Z
// Checks if this object is an instance of the given class or derived from it
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis,           // RCX = this pointer
    const CRuntimeClass* pClass     // RDX = class to check against
) {
    if (!pThis || !pClass) {
        return FALSE;
    }

    // Get this object's runtime class by calling through the vtable
    // In a real scenario, we'd call pThis->GetRuntimeClass() virtually
    // But since this IS CObject's implementation, we need to be careful
    // The actual virtual dispatch happens at the call site

    // For now, assume pThis is exactly a CObject (caller handles vtable dispatch)
    const CRuntimeClass* pThisClass = &CObject::classCObject;

    // Walk the inheritance chain
    while (pThisClass != nullptr) {
        if (pThisClass == pClass) {
            return TRUE;
        }
        pThisClass = pThisClass->m_pBaseClass;
    }

    return FALSE;
}

// CObject::IsSerializable() - const member function
// Symbol: ?IsSerializable@CObject@@QBAHXZ
// Returns TRUE if this class supports serialization
extern "C" int MS_ABI impl__IsSerializable_CObject__QEBAHXZ(
    const CObject* pThis  // RCX = this pointer
) {
    (void)pThis;
    // CObject itself is not serializable (m_wSchema == 0xFFFF)
    // Derived classes that are serializable have m_wSchema != 0xFFFF
    // For CObject base implementation, always return FALSE
    return FALSE;
}

// CObject::Serialize() - virtual member function
// Symbol: ?Serialize@CObject@@UEAAXAEAVCArchive@@@Z (x64)
// Base implementation does nothing
extern "C" void MS_ABI impl__Serialize_CObject__UEAAXAEAVCArchive___Z(
    CObject* pThis,      // RCX = this pointer
    CArchive* ar         // RDX = archive
) {
    (void)pThis;
    (void)ar;
    // Base CObject::Serialize does nothing
}

// C++ implementation of Serialize is already provided inline in afx.h
// The stub above handles the ABI export with mangled name

// =============================================================================
// CRuntimeClass Methods
// =============================================================================

// CRuntimeClass::CreateObject() - instance method
// Symbol: ?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ (x64)
// Creates a new instance of this class using the factory function
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(
    CRuntimeClass* pThis  // RCX = this (CRuntimeClass*)
) {
    if (!pThis || !pThis->m_pfnCreateObject) {
        return nullptr;
    }
    return pThis->m_pfnCreateObject();
}

// CRuntimeClass::CreateObject(const wchar_t*) - static method
// Symbol: ?CreateObject@CRuntimeClass@@SAPAVCObject@@PB_W@Z
// Creates object by class name (wide string)
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEB_W_Z(
    const wchar_t* lpszClassName  // RCX = class name (wide)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Convert wide string to narrow for comparison
    char narrowName[256];
    int i = 0;
    while (lpszClassName[i] && i < 255) {
        narrowName[i] = (char)lpszClassName[i];
        i++;
    }
    narrowName[i] = '\0';

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, narrowName) == 0) {
            if (pClass->m_pfnCreateObject) {
                return pClass->m_pfnCreateObject();
            }
            return nullptr;
        }
    }

    return nullptr;
}

// CRuntimeClass::CreateObject(const char*) - static method
// Symbol: ?CreateObject@CRuntimeClass@@SAPAVCObject@@PBD@Z
// Creates object by class name (narrow string)
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__SAPEAVCObject__PEBD_Z(
    const char* lpszClassName  // RCX = class name (narrow)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
            if (pClass->m_pfnCreateObject) {
                return pClass->m_pfnCreateObject();
            }
            return nullptr;
        }
    }

    return nullptr;
}

// CRuntimeClass::FromName(const wchar_t*) - static method
// Symbol: ?FromName@CRuntimeClass@@SAPAU1@PB_W@Z
// Finds CRuntimeClass by class name (wide string)
extern "C" CRuntimeClass* MS_ABI impl__FromName_CRuntimeClass__SAPEAU1_PEB_W_Z(
    const wchar_t* lpszClassName  // RCX = class name (wide)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Convert wide string to narrow for comparison
    char narrowName[256];
    int i = 0;
    while (lpszClassName[i] && i < 255) {
        narrowName[i] = (char)lpszClassName[i];
        i++;
    }
    narrowName[i] = '\0';

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, narrowName) == 0) {
            return pClass;
        }
    }

    return nullptr;
}

// CRuntimeClass::FromName(const char*) - static method
// Symbol: ?FromName@CRuntimeClass@@SAPAU1@PBD@Z
// Finds CRuntimeClass by class name (narrow string)
extern "C" CRuntimeClass* MS_ABI impl__FromName_CRuntimeClass__SAPEAU1_PEBD_Z(
    const char* lpszClassName  // RCX = class name (narrow)
) {
    if (!lpszClassName) {
        return nullptr;
    }

    InitializeClasses();

    // Search the registered classes
    for (CRuntimeClass* pClass : GetClassRegistry()) {
        if (pClass->m_lpszClassName && strcmp(pClass->m_lpszClassName, lpszClassName) == 0) {
            return pClass;
        }
    }

    return nullptr;
}

// CRuntimeClass::IsDerivedFrom() - const member function
// Symbol: ?IsDerivedFrom@CRuntimeClass@@QBAHPBU1@@Z
// Checks if this class is derived from another class
extern "C" int MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(
    const CRuntimeClass* pThis,     // RCX = this (CRuntimeClass*)
    const CRuntimeClass* pBaseClass // RDX = base class to check
) {
    if (!pThis || !pBaseClass) {
        return FALSE;
    }

    // Walk the inheritance chain
    const CRuntimeClass* pClass = pThis;
    while (pClass != nullptr) {
        if (pClass == pBaseClass) {
            return TRUE;
        }
        pClass = pClass->m_pBaseClass;
    }

    return FALSE;
}

// CRuntimeClass::Load() - static method
// Symbol: ?Load@CRuntimeClass@@SAPAU1@AAVCArchive@@PAI@Z
// Loads CRuntimeClass from archive (for serialization)
extern "C" CRuntimeClass* MS_ABI impl__Load_CRuntimeClass__SAPEAU1_AEAVCArchive__PEAI_Z(
    CArchive* ar,           // RCX = archive
    unsigned int* pwSchemaNum  // RDX = schema number output
) {
    (void)ar;
    (void)pwSchemaNum;
    // Stub: serialization not yet implemented
    // Real implementation would read class name from archive and look it up
    return nullptr;
}

// CRuntimeClass::Store() - const member function
// Symbol: ?Store@CRuntimeClass@@QBAXAAVCArchive@@@Z
// Stores CRuntimeClass to archive (for serialization)
extern "C" void MS_ABI impl__Store_CRuntimeClass__QEBAXAEAVCArchive___Z(
    const CRuntimeClass* pThis,  // RCX = this
    CArchive* ar                 // RDX = archive
) {
    (void)pThis;
    (void)ar;
    // Stub: serialization not yet implemented
    // Real implementation would write class name and schema to archive
}

// =============================================================================
// Helper Functions
// =============================================================================

// AfxClassInit - called during static initialization to register classes
// Symbol: ?AfxClassInit@@YAXPAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__AfxClassInit__YAXPEAUCRuntimeClass___Z(
    CRuntimeClass* pNewClass  // RCX = class to register
) {
    RegisterRuntimeClass(pNewClass);
}

// AfxDynamicDownCast - dynamic_cast equivalent for MFC
// Symbol: ?AfxDynamicDownCast@@YAPAVCObject@@PAUCRuntimeClass@@PAV1@@Z
// Note: This is already in the mapping but we implement it here for completeness
extern "C" CObject* MS_ABI impl__AfxDynamicDownCast__YAPEAVCObject__PEAUCRuntimeClass__PEAV1__Z(
    CRuntimeClass* pClass,  // RCX = target class
    CObject* pObject        // RDX = object to cast
) {
    if (!pObject || !pClass) {
        return nullptr;
    }

    // Get the object's runtime class through vtable
    // For now, we assume caller handles this properly
    // In real MFC, this would call pObject->GetRuntimeClass() virtually

    // Simplified: just return the object (caller responsibility)
    // Real implementation needs vtable access
    return pObject;
}
