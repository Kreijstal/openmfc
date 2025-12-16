#include "openmfc/eh_rtti.h"
#include <stddef.h>

// Helper macro for section placement
// MSVC: use const for .rdata placement (compiler handles it automatically)
// GCC: use section attribute
#if defined(_MSC_VER)
  #define RDATA const
#elif defined(__GNUC__)
  #define RDATA const __attribute__((section(".rdata")))
#else
  #define RDATA const
#endif

#ifndef __stdcall
  #ifdef _MSC_VER
    // MSVC already defines __stdcall
  #elif defined(__GNUC__)
    #define __stdcall __attribute__((stdcall))
  #else
    #define __stdcall
  #endif
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
static RDATA struct {
    int n;
    const CatchableType* types[1];
} CTA_CMemoryException_struct = {
    0,  /* n */
    { 0 }  /* types (empty, placeholder) */
};

static RDATA struct {
    int n;
    const CatchableType* types[1];
} CTA_CFileException_struct = {
    0,  /* n */
    { 0 }  /* types (empty, placeholder) */
};


// ThrowInfos
RDATA ThrowInfo TI_CMemoryException = {
    0,  /* attributes */
    0,  /* pmfnUnwind */
    0,  /* pForwardCompat */
    (const CatchableTypeArray*)&CTA_CMemoryException_struct  /* pCatchableTypeArray */
};

RDATA ThrowInfo TI_CFileException = {
    0,  /* attributes */
    0,  /* pmfnUnwind */
    0,  /* pForwardCompat */
    (const CatchableTypeArray*)&CTA_CFileException_struct  /* pCatchableTypeArray */
};

