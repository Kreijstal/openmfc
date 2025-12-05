#pragma once
#include <cstddef>
#include <cstdint>

// MFC CObject and CRuntimeClass - ABI compatible implementation
//
// CObject is the root of most MFC classes and provides:
// - Runtime type identification (RTTI)
// - Dynamic object creation
// - Serialization support
//
// sizeof(CObject) = 8 (just vptr on x64)

// Calling convention macros for cross-platform
#if defined(_MSC_VER) || defined(__MINGW32__)
    #define AFXAPI __stdcall
#else
    #define AFXAPI
#endif

class CObject;
class CArchive;

// CRuntimeClass - runtime type information structure
// This structure is used for MFC's own RTTI system (separate from C++ RTTI)
struct CRuntimeClass {
    const char* m_lpszClassName;      // Class name string
    int m_nObjectSize;                // sizeof(class)
    unsigned int m_wSchema;           // Schema number for serialization (-1 if not serializable)
    CObject* (AFXAPI *m_pfnCreateObject)();  // Factory function (NULL if not creatable)
    CRuntimeClass* m_pBaseClass;      // Pointer to base class CRuntimeClass (NULL for CObject)
    
    // Helper methods
    CObject* CreateObject() const {
        if (m_pfnCreateObject != nullptr) {
            return m_pfnCreateObject();
        }
        return nullptr;
    }
    
    bool IsDerivedFrom(const CRuntimeClass* pBaseClass) const {
        const CRuntimeClass* pClass = this;
        while (pClass != nullptr) {
            if (pClass == pBaseClass) {
                return true;
            }
            pClass = pClass->m_pBaseClass;
        }
        return false;
    }
};

// CObject - base class for most MFC classes
class CObject {
public:
    // Virtual destructor
    virtual ~CObject() = default;
    
    // Runtime class information
    virtual CRuntimeClass* GetRuntimeClass() const {
        return GetThisClass();
    }
    
    // Static runtime class access
    static CRuntimeClass* GetThisClass() {
        return &classCObject;
    }
    
    // Type checking
    bool IsKindOf(const CRuntimeClass* pClass) const {
        CRuntimeClass* pThisClass = GetRuntimeClass();
        return pThisClass->IsDerivedFrom(pClass);
    }
    
    // Serialization (stub - needs CArchive implementation)
    virtual void Serialize(CArchive& ar) {
        (void)ar; // Not implemented yet
    }
    
    // Diagnostic support (simplified)
    virtual void AssertValid() const {}
    virtual void Dump() const {}
    
    // Static CRuntimeClass for CObject
    static CRuntimeClass classCObject;
    
protected:
    // Protected constructors (CObject shouldn't be directly instantiated in MFC)
    CObject() = default;
    CObject(const CObject&) = default;
    CObject& operator=(const CObject&) = default;
};

// Static assertions to verify ABI compatibility
static_assert(sizeof(CObject) == 8, "CObject must be 8 bytes (vptr only)");

// DECLARE_DYNAMIC - adds runtime class support to a class
// Use in class declaration (public section)
#define DECLARE_DYNAMIC(class_name) \
public: \
    static CRuntimeClass class##class_name; \
    static CRuntimeClass* GetThisClass() { return &class##class_name; } \
    virtual CRuntimeClass* GetRuntimeClass() const override { return GetThisClass(); }

// DECLARE_DYNCREATE - adds runtime class + factory support
#define DECLARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name) \
    static CObject* AFXAPI CreateObject() { return new class_name; }

// IMPLEMENT_DYNAMIC - implements runtime class (put in .cpp file)
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        nullptr, \
        &base_class_name::class##base_class_name \
    };

// IMPLEMENT_DYNCREATE - implements runtime class with factory
#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, \
        sizeof(class_name), \
        0xFFFF, \
        &class_name::CreateObject, \
        &base_class_name::class##base_class_name \
    };

// RUNTIME_CLASS macro - get CRuntimeClass pointer for a class
#define RUNTIME_CLASS(class_name) (&class_name::class##class_name)

// Implementation of CObject's static runtime class
// Note: This should be in a .cpp file normally, but for header-only we inline it
#ifndef COBJECT_IMPL_DEFINED
#define COBJECT_IMPL_DEFINED
inline CRuntimeClass CObject::classCObject = {
    "CObject",
    sizeof(CObject),
    0xFFFF,
    nullptr,
    nullptr
};
#endif
