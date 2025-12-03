#pragma once

// Minimal, clean-room subset needed for Phase 0B (CObject).
// This is intentionally tiny and will grow with later phases.

#include <stddef.h>
#include <stdint.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#if defined(OPENMFC_EXPORTS)
    #define OPENMFC_API __declspec(dllexport)
#else
    #define OPENMFC_API __declspec(dllimport)
#endif

#ifndef AFX_PASCAL
#define AFX_PASCAL __stdcall
#endif

class CObject;

struct CRuntimeClass {
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;
    CObject* (AFX_PASCAL* m_pfnCreateObject)();
    CRuntimeClass* m_pBaseClass;
};

class CArchive; // forward declaration placeholder

class OPENMFC_API CObject {
public:
    CObject();
    virtual ~CObject();

    virtual CRuntimeClass* GetRuntimeClass() const;
    virtual void Serialize(CArchive& ar);
    BOOL IsKindOf(const CRuntimeClass* pClass) const;

    void* operator new(size_t nSize);
    void operator delete(void* p);

    static CObject* AFX_PASCAL CreateObject();
};
