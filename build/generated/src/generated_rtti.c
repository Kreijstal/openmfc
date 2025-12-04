#include "openmfc/eh_rtti.h"
#include <stddef.h>

// Helper macro for section placement
#ifdef _WIN32
#define RDATA __attribute__((section(".rdata")))
#else
#define RDATA __attribute__((section(".data.rel.ro")))
#endif

#ifndef __stdcall
#define __stdcall
#endif

// External copy constructors (thunks)
// These will be implemented in exceptions.c or assembly
extern void __stdcall CException_CopyCtor(void* dest, void* src);
// For now, we might use a generic one or specific ones. 
// The harvester didn't give us the copy function name, so we might default to null or a generic one.
// In a real scenario, we'd need to know which copy ctor to use.
// For this phase, we'll use NULL or a stub.

// TypeDescriptors


// CatchableTypes


// CatchableTypeArrays
RDATA static const struct { int n; const CatchableType* types[0]; } CTA_CMemoryException_struct = {
    0,
    {  }
};
RDATA static const struct { int n; const CatchableType* types[0]; } CTA_CFileException_struct = {
    0,
    {  }
};

// ThrowInfos
RDATA const ThrowInfo TI_CMemoryException = {
    0, // attributes
    0, // pmfnUnwind
    0, // pForwardCompat
    (const CatchableTypeArray*)&CTA_CMemoryException_struct
};
RDATA const ThrowInfo TI_CFileException = {
    0, // attributes
    0, // pmfnUnwind
    0, // pForwardCompat
    (const CatchableTypeArray*)&CTA_CFileException_struct
};
