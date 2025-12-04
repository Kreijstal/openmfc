#pragma once
#include <cstdint>

// ABI structures mirrored from MSVC runtime documentation.
struct PMD {
    int mdisp;
    int pdisp;
    int vdisp;
};

struct CatchableType {
    unsigned int properties;
    void* pType;           // TypeDescriptor*
    PMD thisDisplacement;
    int sizeOrOffset;
    void (*copyFunction)(void);
};

struct CatchableTypeArray {
    int nCatchableTypes;
    CatchableType* types[1]; // variable length
};

struct ThrowInfo {
    unsigned int attributes;
    void (*pmfnUnwind)(void);
    int (*pForwardCompat)(void);
    CatchableTypeArray* pCatchableTypeArray;
};

struct ExceptionDump {
    const void* throw_info;
    const void* obj;
};

