#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// MSVC RTTI/EH structures
typedef struct TypeDescriptor {
    const void* pVFTable;
    void* spare;
    char name[1]; // Variable length (sized differently per instance)
} TypeDescriptor;

typedef struct PMD {
    int mdisp;
    int pdisp;
    int vdisp;
} PMD;

typedef struct CatchableType {
    unsigned int properties;
    const TypeDescriptor* pType;
    PMD thisDisplacement;
    int sizeOrOffset;
    void (*copyFunction)(void);
} CatchableType;

// Note: CatchableTypeArray uses a fixed-size placeholder in the typedef.
// Actual instances are created with correctly-sized arrays.
typedef struct CatchableTypeArray {
    int nCatchableTypes;
    const CatchableType* arrayOfTypes[1]; // Placeholder; actual size varies
} CatchableTypeArray;

typedef struct ThrowInfo {
    unsigned int attributes;
    void (*pmfnUnwind)(void);
    int (*pForwardCompat)(void);
    const CatchableTypeArray* pCatchableTypeArray;
} ThrowInfo;

// Forward declarations
extern const ThrowInfo TI_CMemoryException;
extern const ThrowInfo TI_CFileException;

#ifdef __cplusplus
}
#endif
