#pragma once
// Shared internals of the former mfc_exceptions.cpp translation unit.
// Definitions live in detail/MfcExceptionsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcexceptions {} } }
using namespace openmfc::detail::mfcexceptions;
// Legacy DAO exception support. Other exception families own their actual C++
// classes, metadata and typed throws in core/exceptions.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxdao.h"
#include "openmfc/afxinet.h"
#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <new>
#include <string>
#include <unordered_map>

// Implement CException runtime class

// Create MSVC symbol alias for CException::classCException
#ifdef __GNUC__
asm(".globl \"?classCException@CException@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCException@CException@@2UCRuntimeClass@@A\", _ZN10CException15classCExceptionE\n");
#endif

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
























#ifdef __GNUC__
asm(".globl \"?GetErrorMessage@CException@@UEBAHPEA_WIPEAI@Z\"\n"
    ".set \"?GetErrorMessage@CException@@UEBAHPEA_WIPEAI@Z\", impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z\n");
#endif

extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGII_Z(
    WORD wCode, UINT nDescriptionID, UINT nHelpID
);
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(
    WORD wCode, const wchar_t* lpszDescription, UINT nHelpID
);

void AFXAPI AfxThrowOleException(LONG sc);

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID);

void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID);

// =============================================================================
// CException::GetThisClass Implementation
// =============================================================================






// =============================================================================
// Missing Exception Class Definitions
// =============================================================================

// =============================================================================
// MSVC Exception Structures (x64)
// =============================================================================

#pragma pack(push, 4)

struct TypeDescriptor {
    const void* pVFTable;
    void* spare;
    char name[1];
};

struct CatchableType {
    uint32_t properties;
    int32_t pType;
    int32_t thisDisplacement_mdisp;
    int32_t thisDisplacement_pdisp;
    int32_t thisDisplacement_vdisp;
    int32_t sizeOrOffset;
    int32_t copyFunction;
};

struct CatchableTypeArray {
    uint32_t nCatchableTypes;
    int32_t arrayOfCatchableTypes[1];
};

struct ThrowInfo {
    uint32_t attributes;
    int32_t pmfnUnwind;
    int32_t pForwardCompat;
    int32_t pCatchableTypeArray;
};

#pragma pack(pop)

// =============================================================================
// Runtime initialization
// =============================================================================

typedef void (MS_ABI *CxxThrowExceptionFunc)(void* pExceptionObject, ThrowInfo* pThrowInfo);

namespace openmfc { namespace detail { namespace mfcexceptions {
extern CxxThrowExceptionFunc g_pCxxThrowException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern HMODULE g_hVCRuntime;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern uintptr_t g_imageBase;
} } }
static const void* g_typeInfoVFT = nullptr;
namespace openmfc { namespace detail { namespace mfcexceptions {
extern bool g_initialized;
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
uintptr_t GetOurImageBase();
} } }

#define TO_RVA(addr) ((int32_t)((uintptr_t)(addr) - g_imageBase))

// Dummy type_info vftable
namespace openmfc { namespace detail { namespace mfcexceptions {
void* MS_ABI dummy_dtor(void* p);
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
int MS_ABI dummy_eq(const void* pThis, const void* pOther);
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
int MS_ABI dummy_ne(const void* pThis, const void* pOther);
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
const char* MS_ABI dummy_name(void*);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern void* g_dummyTypeInfoVFTable[];
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
const void* GetTypeInfoVFTable();
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
bool InitExceptionSystem();
} } }

// =============================================================================
// MACRO: Define RTTI structures for an exception type
// =============================================================================

#define DEFINE_EXCEPTION_RTTI(ExcName, TypeDescLen) \
    static struct { \
        const void* pVFTable; \
        void* spare; \
        char name[TypeDescLen]; \
    } TD_##ExcName = { nullptr, nullptr, ".PEAV" #ExcName "@@" }; \
    \
    static CatchableType CT_##ExcName = { 1, 0, 0, -1, 0, 8, 0 }; \
    \
    static struct { \
        uint32_t nCatchableTypes; \
        int32_t types[3]; \
    } CTA_##ExcName = { 3, { 0, 0, 0 } }; \
    \
    static ThrowInfo TI_##ExcName = { 0, 0, 0, 0 };

// Base types (shared)
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CException_t {
    const void* pVFTable;
    void* spare;
    char name[20];
};
extern TD_CException_t TD_CException;
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CObject_t {
    const void* pVFTable;
    void* spare;
    char name[16];
};
extern TD_CObject_t TD_CObject;
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CObject;
} } }

// Legacy metadata for the remaining exception families.
// CDaoException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CDaoException_t {
    const void* pVFTable;
    void* spare;
    char name[25];
};
extern TD_CDaoException_t TD_CDaoException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CDaoException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CDaoException_t { uint32_t n; int32_t t[3]; };
extern CTA_CDaoException_t CTA_CDaoException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CDaoException;
} } }

// =============================================================================
// Initialize RTTI for all exception types
// =============================================================================

namespace openmfc { namespace detail { namespace mfcexceptions {
extern bool g_rttiInitialized;
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
void InitAllRTTI();
} } }

template<typename T>
static void ThrowNew(T* pException, ThrowInfo* pThrowInfo) {
    if (!InitExceptionSystem()) { abort(); }
    InitAllRTTI();
    g_pCxxThrowException(&pException, pThrowInfo);
    abort();
}

// =============================================================================
// Public API: Exception Throwing Functions
// =============================================================================

// AfxThrowMemoryException - void()
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
} } }

// C++ entry point for in-repo code (delegates to the MSVC-ABI export impl).
void AFXAPI AfxThrowMemoryException();

// AfxThrowNotSupportedException - void()
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowNotSupportedException__YAXXZ();
} } }

// AfxThrowResourceException - void()
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();
} } }

// AfxThrowUserException - void()
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ();

// AfxThrowInvalidArgException - void()
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();

// AfxThrowFileException - void(int cause, LONG lOsError, const wchar_t* lpszFileName)
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(
    int cause, LONG lOsError, const wchar_t* lpszFileName
);

// AfxThrowArchiveException - void(int cause, const wchar_t* lpszArchiveName)
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName
);
} } }

// AfxThrowOleException - void(HRESULT sc)
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);

// AfxThrowOleDispatchException - void(WORD wCode, UINT nDescriptionID, UINT nHelpID)
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGII_Z(
    WORD wCode, UINT nDescriptionID, UINT nHelpID
);

// AfxThrowOleDispatchException - void(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID)
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(
    WORD wCode, const wchar_t* lpszDescription, UINT nHelpID
);









// AfxThrowDBException - void(short nRetCode, CDatabase* pdb, void* hstmt)
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(
    short nRetCode, void* pdb, void* hstmt
);
} } }

// AfxThrowDaoException - void(int nAfxDaoError, SCODE scode)
// Note: Signature might vary, checking ordinal mapping would be ideal but assuming standard
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowDaoException__YAXHJ_Z(
    int nAfxDaoError, SCODE scode
);
} } }

// AfxAbort - terminates the application
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxAbort__YAXXZ();
} } }
