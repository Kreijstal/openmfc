// MFC Exception Implementation for OpenMFC
//
// This implements AfxThrowMemoryException and related functions using
// MSVC-compatible C++ exception structures so that MSVC-compiled code
// can catch them with try/catch.
//
// Key insight: We call _CxxThrowException from vcruntime140.dll with
// manually constructed RTTI structures that match MSVC's format.

#include <windows.h>
#include <cstdint>
#include <cstdio>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// MSVC Exception Structures (x64)
// =============================================================================

// All pointers in these structures are RVAs (relative to image base) on x64

#pragma pack(push, 4)

// Type descriptor - contains RTTI type name
struct TypeDescriptor {
    const void* pVFTable;     // Pointer to type_info vftable
    void* spare;              // For runtime use (caching)
    char name[1];             // Decorated name: .?AVClassName@@
};

// Catchable type - describes one type in the hierarchy
struct CatchableType {
    uint32_t properties;      // 0 = simple type, 1 = simple inheritance, etc.
    int32_t pType;            // RVA to TypeDescriptor
    int32_t thisDisplacement_mdisp;  // Member displacement
    int32_t thisDisplacement_pdisp;  // Vbtable displacement (-1 if none)
    int32_t thisDisplacement_vdisp;  // Displacement within vbtable
    int32_t sizeOrOffset;     // Object size
    int32_t copyFunction;     // RVA to copy constructor (0 = use memcpy)
};

// Catchable type array - list of catchable types
struct CatchableTypeArray {
    uint32_t nCatchableTypes;
    int32_t arrayOfCatchableTypes[1];  // RVAs to CatchableType
};

// Throw info - passed to _CxxThrowException
struct ThrowInfo {
    uint32_t attributes;      // 0 = normal, 1 = const, etc.
    int32_t pmfnUnwind;       // RVA to destructor
    int32_t pForwardCompat;   // RVA to forward compat handler (0)
    int32_t pCatchableTypeArray;  // RVA to CatchableTypeArray
};

#pragma pack(pop)

// =============================================================================
// Import _CxxThrowException from vcruntime140.dll
// =============================================================================

// _CxxThrowException signature for x64
typedef void (MS_ABI *CxxThrowExceptionFunc)(void* pExceptionObject, ThrowInfo* pThrowInfo);

static CxxThrowExceptionFunc g_pCxxThrowException = nullptr;
static HMODULE g_hVCRuntime = nullptr;
static uintptr_t g_imageBase = 0;

// Get the base address of our DLL
static uintptr_t GetOurImageBase() {
    HMODULE hModule;
    if (GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&GetOurImageBase,
            &hModule)) {
        return (uintptr_t)hModule;
    }
    return 0;
}

// Convert absolute address to RVA
#define TO_RVA(addr) ((int32_t)((uintptr_t)(addr) - g_imageBase))

// Initialize exception throwing
static bool InitExceptionThrowing() {
    if (g_pCxxThrowException) return true;

    g_imageBase = GetOurImageBase();
    if (!g_imageBase) {
        fprintf(stderr, "OpenMFC: Failed to get image base\n");
        return false;
    }

    // Try vcruntime140.dll first (VS2015+), then msvcrt.dll
    g_hVCRuntime = LoadLibraryA("vcruntime140.dll");
    if (!g_hVCRuntime) {
        g_hVCRuntime = LoadLibraryA("msvcrt.dll");
    }

    if (!g_hVCRuntime) {
        fprintf(stderr, "OpenMFC: Failed to load vcruntime\n");
        return false;
    }

    g_pCxxThrowException = (CxxThrowExceptionFunc)GetProcAddress(g_hVCRuntime, "_CxxThrowException");
    if (!g_pCxxThrowException) {
        fprintf(stderr, "OpenMFC: Failed to find _CxxThrowException\n");
        return false;
    }

    return true;
}

// =============================================================================
// MSVC type_info vftable
// =============================================================================

// We need a pointer to type_info's vftable. This is tricky because we're
// building with MinGW. We can get it from an existing type_info in vcruntime.
static const void* GetTypeInfoVFTable() {
    // The type_info vftable is exported as ??_7type_info@@6B@
    if (!g_hVCRuntime) {
        g_hVCRuntime = LoadLibraryA("vcruntime140.dll");
    }
    if (g_hVCRuntime) {
        void* vftable = (void*)GetProcAddress(g_hVCRuntime, "??_7type_info@@6B@");
        if (vftable) return vftable;
    }
    return nullptr;
}

// =============================================================================
// CMemoryException RTTI Structures
// =============================================================================

// These must be in a section that persists (static storage)
// The RVAs are computed at runtime

// Type descriptors for pointer types
// When throwing a pointer (CMemoryException*), we need .PEAV prefix
// .PEAV = Pointer to class type (E=__ptr64 on x64, A=no cv-qualifiers, V=class)
static struct {
    const void* pVFTable;
    void* spare;
    char name[28];  // .PEAVCMemoryException@@\0
} TD_CMemoryException = { nullptr, nullptr, ".PEAVCMemoryException@@" };

static struct {
    const void* pVFTable;
    void* spare;
    char name[24];  // .PEAVCException@@\0
} TD_CException = { nullptr, nullptr, ".PEAVCException@@" };

static struct {
    const void* pVFTable;
    void* spare;
    char name[20];  // .PEAVCObject@@\0
} TD_CObject = { nullptr, nullptr, ".PEAVCObject@@" };

// Catchable types for pointer types
// For pointers, size is sizeof(void*) = 8 on x64
static CatchableType CT_CMemoryException = {
    1,              // properties: 1 = simple type (pointer)
    0,              // pType: RVA to TD_CMemoryException (set at runtime)
    0,              // mdisp
    -1,             // pdisp
    0,              // vdisp
    8,              // size: sizeof(CMemoryException*) = 8 on x64
    0               // copyFunction: 0 = memcpy
};

static CatchableType CT_CException = {
    1,              // properties: 1 = simple type (pointer)
    0,              // pType: RVA to TD_CException (set at runtime)
    0,              // mdisp
    -1,             // pdisp
    0,              // vdisp
    8,              // size: sizeof(CException*) = 8 on x64
    0               // copyFunction
};

static CatchableType CT_CObject = {
    1,              // properties: 1 = simple type (pointer)
    0,              // pType: RVA to TD_CObject (set at runtime)
    0,              // mdisp
    -1,             // pdisp
    0,              // vdisp
    8,              // size: sizeof(CObject*) = 8 on x64
    0               // copyFunction
};

// Catchable type array for CMemoryException (can catch as CMemoryException, CException, or CObject)
static struct {
    uint32_t nCatchableTypes;
    int32_t types[3];
} CTA_CMemoryException = { 3, { 0, 0, 0 } };

// ThrowInfo for CMemoryException
static ThrowInfo TI_CMemoryException = {
    0,              // attributes
    0,              // pmfnUnwind: destructor RVA (set at runtime)
    0,              // pForwardCompat
    0               // pCatchableTypeArray: RVA (set at runtime)
};

// =============================================================================
// Exception Object Layout
// =============================================================================

// Minimal CObject-compatible layout
struct CObjectData {
    const void* vptr;     // Virtual function table pointer
};

// Minimal CException-compatible layout
struct CExceptionData {
    const void* vptr;
    int m_bAutoDelete;    // MFC uses BOOL (int)
};

// CMemoryException is same as CException (no extra data)
struct CMemoryExceptionData {
    const void* vptr;
    int m_bAutoDelete;
};

// =============================================================================
// Virtual Function Tables
// =============================================================================

// We need vtables that point to our implementations
// For now, minimal stubs

static void MS_ABI CMemoryException_Destructor(CMemoryExceptionData* pThis) {
    // Destructor - does nothing for now
}

static void* MS_ABI CMemoryException_GetRuntimeClass(CMemoryExceptionData* pThis) {
    return nullptr;  // TODO: return proper CRuntimeClass
}

// Virtual function table for CMemoryException
// Order must match MSVC's vtable layout for CObject -> CException -> CMemoryException
static const void* VFTable_CMemoryException[] = {
    (void*)CMemoryException_GetRuntimeClass,  // GetRuntimeClass
    (void*)CMemoryException_Destructor,       // Destructor (scalar)
    // Add more virtual functions as needed
};

// =============================================================================
// Setup RTTI structures (call once)
// =============================================================================

static bool g_rttiInitialized = false;

static void InitRTTI() {
    if (g_rttiInitialized) return;

    const void* typeInfoVFT = GetTypeInfoVFTable();
    if (!typeInfoVFT) {
        fprintf(stderr, "OpenMFC: Failed to get type_info vftable\n");
        return;
    }

    // Set vftable pointers in type descriptors
    TD_CMemoryException.pVFTable = typeInfoVFT;
    TD_CException.pVFTable = typeInfoVFT;
    TD_CObject.pVFTable = typeInfoVFT;

    // Compute RVAs
    g_imageBase = GetOurImageBase();

    // Set RVAs in catchable types
    CT_CMemoryException.pType = TO_RVA(&TD_CMemoryException);
    CT_CException.pType = TO_RVA(&TD_CException);
    CT_CObject.pType = TO_RVA(&TD_CObject);

    // Set RVAs in catchable type array
    CTA_CMemoryException.types[0] = TO_RVA(&CT_CMemoryException);
    CTA_CMemoryException.types[1] = TO_RVA(&CT_CException);
    CTA_CMemoryException.types[2] = TO_RVA(&CT_CObject);

    // Set RVAs in throw info
    TI_CMemoryException.pmfnUnwind = TO_RVA(&CMemoryException_Destructor);
    TI_CMemoryException.pCatchableTypeArray = TO_RVA(&CTA_CMemoryException);

    g_rttiInitialized = true;
}

// =============================================================================
// Public API: AfxThrowMemoryException
// =============================================================================

// Static exception object (MFC often uses a single static instance)
static CMemoryExceptionData g_memoryExceptionObject = { VFTable_CMemoryException, 0 };

// Pointer to the exception object - this is what we actually throw
// When throwing a pointer type, the exception object IS the pointer value
static CMemoryExceptionData* g_pMemoryException = &g_memoryExceptionObject;

extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    if (!InitExceptionThrowing()) {
        fprintf(stderr, "OpenMFC: Cannot throw - exception init failed\n");
        abort();
    }

    InitRTTI();

    // Set up the exception object
    g_memoryExceptionObject.vptr = VFTable_CMemoryException;
    g_memoryExceptionObject.m_bAutoDelete = 0;  // Don't auto-delete static object
    g_pMemoryException = &g_memoryExceptionObject;

    // Throw the pointer!
    // _CxxThrowException takes:
    //   - Pointer to the exception object (which is the pointer value itself for pointer types)
    //   - Pointer to ThrowInfo
    g_pCxxThrowException(&g_pMemoryException, &TI_CMemoryException);

    // Should never reach here
    abort();
}

// =============================================================================
// Other exception stubs (not yet implemented with real throwing)
// =============================================================================

extern "C" void MS_ABI stub__AfxThrowFileException__YAXHJPB_W_Z(int cause, LONG lOsError, const wchar_t* lpszFileName) {
    fprintf(stderr, "AfxThrowFileException: cause=%d, error=%ld (stub)\n", cause, lOsError);
    if (lpszFileName) {
        fprintf(stderr, "  file: %ls\n", lpszFileName);
    }
    // TODO: Implement proper CFileException throwing
}

extern "C" void MS_ABI stub__AfxThrowResourceException__YAXXZ() {
    fprintf(stderr, "AfxThrowResourceException (stub)\n");
    // TODO: Implement proper CResourceException throwing
}

extern "C" void MS_ABI stub__AfxThrowNotSupportedException__YAXXZ() {
    fprintf(stderr, "AfxThrowNotSupportedException (stub)\n");
    // TODO: Implement proper CNotSupportedException throwing
}

extern "C" void MS_ABI stub__AfxThrowUserException__YAXXZ() {
    fprintf(stderr, "AfxThrowUserException (stub)\n");
    // TODO: Implement proper CUserException throwing
}

extern "C" void MS_ABI stub__AfxThrowInvalidArgException__YAXXZ() {
    fprintf(stderr, "AfxThrowInvalidArgException (stub)\n");
    // TODO: Implement proper CInvalidArgException throwing
}

// AfxAbort - terminates the application
extern "C" void MS_ABI stub__AfxAbort__YAXXZ() {
    fprintf(stderr, "AfxAbort: Terminating application\n");
    abort();
}
