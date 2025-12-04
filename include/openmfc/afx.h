#pragma once

// Minimal core MFC forward declarations for OpenMFC scaffolding.
class CRuntimeClass;

class CObject {
public:
    virtual ~CObject() = default;
    virtual CRuntimeClass* GetRuntimeClass() const;
    virtual void Serialize() {} // placeholder
};

class CRuntimeClass {
public:
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned int m_wSchema;
    CRuntimeClass* m_pBaseClass;
    CObject* (*m_pfnCreateObject)();
};

#define RUNTIME_CLASS(cls) (&cls::class##cls)

