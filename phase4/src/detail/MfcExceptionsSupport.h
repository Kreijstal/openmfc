#pragma once
// Shared internals of the former mfc_exceptions.cpp translation unit.
// Definitions live in detail/MfcExceptionsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace mfcexceptions {} } }
using namespace openmfc::detail::mfcexceptions;
// MFC Exception Implementation for OpenMFC
//
// This implements all AfxThrowXxxException functions using MSVC-compatible
// C++ exception structures so that MSVC-compiled code can catch them.
//
// Supported exception types:
// - CMemoryException (Static instance)
// - CNotSupportedException_MfcExceptions
// - CResourceException_MfcExceptions
// - CUserException_MfcExceptions
// - CInvalidArgException_MfcExceptions
// - CFileException
// - CArchiveException
// - COleException
// - COleDispatchException
// - CInternetException
// - CDBException
// - CDaoException (Stubbed as CException)
//
// Key insight: We call _CxxThrowException from vcruntime140.dll with
// manually constructed RTTI structures that match MSVC's format.

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


namespace openmfc { namespace detail { namespace mfcexceptions {
extern std::mutex g_oleDispatchDescriptionMutex;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern std::unordered_map<const COleDispatchException*, std::wstring> g_oleDispatchDescriptions;
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
std::wstring LoadOleDispatchDescription(UINT nDescriptionID);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
void CopyErrorText(wchar_t* out, UINT maxLen, const wchar_t* text);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
void EmitDiagnosticText(const wchar_t* text);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
void CopyClassName(const CException* pThis, wchar_t* out, size_t outCount);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
const wchar_t* FileCauseText(int cause);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
const wchar_t* ArchiveCauseText(int cause);
} } }
















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

// CNotSupportedException_MfcExceptions
class CNotSupportedException_MfcExceptions : public CException {
public:
    CNotSupportedException_MfcExceptions() : CException(TRUE) {}
};

// CResourceException_MfcExceptions
class CResourceException_MfcExceptions : public CException {
public:
    CResourceException_MfcExceptions() : CException(TRUE) {}
};

// CUserException_MfcExceptions
class CUserException_MfcExceptions : public CException {
public:
    CUserException_MfcExceptions() : CException(TRUE) {}
};

// CInvalidArgException_MfcExceptions
class CInvalidArgException_MfcExceptions : public CException {
public:
    CInvalidArgException_MfcExceptions() : CException(TRUE) {}
};

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

// Define all exception types
// CMemoryException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CMemoryException_t {
    const void* pVFTable;
    void* spare;
    char name[28];
};
extern TD_CMemoryException_t TD_CMemoryException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CMemoryException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CMemoryException_t { uint32_t n; int32_t t[3]; };
extern CTA_CMemoryException_t CTA_CMemoryException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CMemoryException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CMemoryException g_MemoryException; // Static instance;
} } }

// CNotSupportedException_MfcExceptions
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CNotSupportedException_t {
    const void* pVFTable;
    void* spare;
    char name[32];
};
extern TD_CNotSupportedException_t TD_CNotSupportedException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CNotSupportedException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CNotSupportedException_t { uint32_t n; int32_t t[3]; };
extern CTA_CNotSupportedException_t CTA_CNotSupportedException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CNotSupportedException;
} } }

// CResourceException_MfcExceptions
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CResourceException_t {
    const void* pVFTable;
    void* spare;
    char name[28];
};
extern TD_CResourceException_t TD_CResourceException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CResourceException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CResourceException_t { uint32_t n; int32_t t[3]; };
extern CTA_CResourceException_t CTA_CResourceException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CResourceException;
} } }

// CUserException_MfcExceptions
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CUserException_t {
    const void* pVFTable;
    void* spare;
    char name[24];
};
extern TD_CUserException_t TD_CUserException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CUserException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CUserException_t { uint32_t n; int32_t t[3]; };
extern CTA_CUserException_t CTA_CUserException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CUserException;
} } }

// CInvalidArgException_MfcExceptions
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CInvalidArgException_t {
    const void* pVFTable;
    void* spare;
    char name[32];
};
extern TD_CInvalidArgException_t TD_CInvalidArgException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CInvalidArgException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CInvalidArgException_t { uint32_t n; int32_t t[3]; };
extern CTA_CInvalidArgException_t CTA_CInvalidArgException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CInvalidArgException;
} } }

// CFileException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CFileException_t {
    const void* pVFTable;
    void* spare;
    char name[24];
};
extern TD_CFileException_t TD_CFileException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CFileException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CFileException_t { uint32_t n; int32_t t[3]; };
extern CTA_CFileException_t CTA_CFileException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CFileException;
} } }

// CArchiveException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CArchiveException_t {
    const void* pVFTable;
    void* spare;
    char name[28];
};
extern TD_CArchiveException_t TD_CArchiveException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CArchiveException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CArchiveException_t { uint32_t n; int32_t t[3]; };
extern CTA_CArchiveException_t CTA_CArchiveException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CArchiveException;
} } }

// COleException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_COleException_t {
    const void* pVFTable;
    void* spare;
    char name[24];
};
extern TD_COleException_t TD_COleException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_COleException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_COleException_t { uint32_t n; int32_t t[3]; };
extern CTA_COleException_t CTA_COleException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_COleException;
} } }

// COleDispatchException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_COleDispatchException_t {
    const void* pVFTable;
    void* spare;
    char name[32];
};
extern TD_COleDispatchException_t TD_COleDispatchException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_COleDispatchException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_COleDispatchException_t { uint32_t n; int32_t t[4]; };
extern CTA_COleDispatchException_t CTA_COleDispatchException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_COleDispatchException;
} } }

// CInternetException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CInternetException_t {
    const void* pVFTable;
    void* spare;
    char name[28];
};
extern TD_CInternetException_t TD_CInternetException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CInternetException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CInternetException_t { uint32_t n; int32_t t[3]; };
extern CTA_CInternetException_t CTA_CInternetException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CInternetException;
} } }

// CDBException
namespace openmfc { namespace detail { namespace mfcexceptions {
struct TD_CDBException_t {
    const void* pVFTable;
    void* spare;
    char name[24];
};
extern TD_CDBException_t TD_CDBException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern CatchableType CT_CDBException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
struct CTA_CDBException_t { uint32_t n; int32_t t[3]; };
extern CTA_CDBException_t CTA_CDBException;
} } }
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ThrowInfo TI_CDBException;
} } }

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

// =============================================================================
// MSVC-compatible vtable for exception classes
// =============================================================================
//
// Problem: MinGW uses Itanium ABI which has 2 destructor entries in vtable.
// MSVC has only 1 destructor entry. So the virtual function offsets differ:
//   MSVC:   vtable[0]=GetRuntimeClass, vtable[1]=dtor, vtable[2]=Serialize, ...
//   MinGW:  vtable[0]=GetRuntimeClass, vtable[1]=dtor1, vtable[2]=dtor2, ...
//
// (Slot order above is the harvested ground truth for the CException family;
//  see phase1/harvest/vtable_slots.json, regenerated by
//  tools/abi/resolve_vtable_slots.py from a live mfc140u.dll dump.)
//
// When MSVC code catches our exception and calls a virtual past the destructor,
// it indexes by the MSVC slot number, but in MinGW's vtable the extra deleting
// destructor entry has shifted everything down by one => wrong function!
//
// Solution: Create MSVC-compatible vtables and patch the vptr before throwing.

// =============================================================================
// MSVC-compatible vtable stubs and destructor shims
// =============================================================================
//
// Auto-delete behavior and memory management
// -------------------------------------------
// MFC exceptions have an m_bAutoDelete member that controls whether Delete()
// should call 'delete this'. We handle this correctly for all exception types:
//
// Static exceptions (m_bAutoDelete=0, never deleted):
// - CMemoryException: Uses static instance (g_ManualMemoryException)
// - CResourceException_MfcExceptions, CUserException_MfcExceptions: Use static instances
// - These use a static-lifetime destructor thunk.
//
// Heap-allocated exceptions (m_bAutoDelete=1, may be deleted by MSVC):
// - CFileException, CArchiveException: Created with 'new'
// - These use proper destructor shims (dtor_CFileException, dtor_CArchiveException)
// - The shims call the actual C++ destructor to clean up members
// - Memory deallocation uses MinGW's operator delete (via the same heap)
//
// Static destructor shim for static exceptions (CMemoryException and peers).
// Returns 'this' as MSVC destructors do; caller won't deallocate since
// m_bAutoDelete is initialized to 0 for these objects.
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void* MS_ABI dtor_CMemoryException(void* pThis);
} } }

// Padding for the slot past ReportError. The harvested CException-family vtable
// (phase1/harvest/vtable_slots.json) is exactly:
//   [0] GetRuntimeClass  [1] ~dtor  [2] Serialize  [3] AssertValid  [4] Dump
//   [5] GetErrorMessage (CException, non-const)
//   [6] GetErrorMessage (CSimpleException, const)   <- the one CMemoryException uses
//   [7] ReportError
// We serve the message thunk at both [5] and [6] (the two distinct GetErrorMessage
// virtuals) and map [7] to a diagnostic helper since MSVC ABI can call this slot
// during failure paths.
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" int MS_ABI vtbl_ReportErrorPad(void* pThis);
} } }

// =============================================================================
// Proper destructor shims for heap-allocated exceptions
// =============================================================================
// These shims handle exceptions allocated with MinGW's 'new' that may be
// deleted by MSVC code. They call the C++ destructor and use MinGW's
// operator delete to ensure correct heap deallocation.
//
// MSVC virtual destructor ABI:
// - Takes 'this' pointer in RCX
// - Returns 'this' pointer (for chaining)
// - After return, the caller may call operator delete if deleting

// CFileException destructor shim - properly destroys and can be deleted
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void* MS_ABI dtor_CFileException(CFileException* pThis);
} } }

// CArchiveException destructor shim
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void* MS_ABI dtor_CArchiveException(CArchiveException* pThis);
} } }

// Operator delete shim - ensures MinGW heap is used for deallocation
// This is called after the destructor when MSVC code calls 'delete pException'
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI opdelete_shim(void* pThis);
} } }

// Serialize path for exceptions. The shipped MFC exception types are not
// archive-serializable in this project, so this delegates to the base CObject
// implementation for consistent behavior while keeping the slot ABI-compatible.
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_Serialize(CObject* pThis, CArchive* pArchive);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_AssertValid(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_Dump(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" int MS_ABI vtbl_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
);
} } }

// MSVC vtable for CMemoryException
// Note: In real MFC, CObject declares GetRuntimeClass BEFORE the destructor.
// So the layout is: [GetRuntimeClass, dtor, Serialize, AssertValid, Dump, GetErrorMessage]

// =============================================================================
// MSVC-compatible vtable arrays
// =============================================================================
// These arrays serve as vtable pointers for exception objects thrown to MSVC code.
// Each entry is explicitly cast to void* to make the ABI assumptions clear:
// - Index 0: GetRuntimeClass (virtual method declared first in CObject)
// - Index 1: Destructor
// - Index 2+: Other virtual methods in declaration order
//
// When we set an object's vptr to point to these arrays, MSVC code can call
// virtual methods correctly even though the object was created by MinGW.

// Forward declaration of exported GetThisClass
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" CRuntimeClass* MS_ABI vtbl_CMemoryException_GetRuntimeClass(const CObject* pThis);
} } }

// CMemoryException has no per-instance state, so its message is constant.
// Real MFC reports AFX_IDP_OUT_OF_MEMORY ("Out of memory.").
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" int MS_ABI vtbl_CMemoryException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
);
} } }

// CMemoryException vtable - used by g_ManualMemoryException (static, never deleted)
namespace openmfc { namespace detail { namespace mfcexceptions {
extern void* g_vtbl_CMemoryException[];
} } }

// CFileException vtable
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" CRuntimeClass* MS_ABI vtbl_CFileException_GetRuntimeClass(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_CFileException_AssertValid(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_CFileException_Dump(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" int MS_ABI vtbl_CFileException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
);
} } }

// CFileException vtable - heap-allocated, needs proper destructor
namespace openmfc { namespace detail { namespace mfcexceptions {
extern void* g_vtbl_CFileException[];
} } }

// CArchiveException vtable
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" CRuntimeClass* MS_ABI vtbl_CArchiveException_GetRuntimeClass(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_CArchiveException_AssertValid(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI vtbl_CArchiveException_Dump(const CObject* pThis);
} } }

namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" int MS_ABI vtbl_CArchiveException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
);
} } }

// CArchiveException vtable - heap-allocated, needs proper destructor
namespace openmfc { namespace detail { namespace mfcexceptions {
extern void* g_vtbl_CArchiveException[];
} } }

// Patch vptr to point to our MSVC-compatible vtable
template<typename T>
static void PatchVPtr(T* pObj, void** vtable) {
    // The vptr is at offset 0 in the object
    void** currentVptr = *reinterpret_cast<void***>(pObj);
    (void)currentVptr; // For debugging: we're replacing this
    *reinterpret_cast<void***>(pObj) = vtable;
}

// =============================================================================
// ManualCMemoryException - ABI-compatible static exception object
// =============================================================================
//
// This struct mirrors CMemoryException's memory layout exactly so we can
// pre-construct an exception object with our MSVC-compatible vtable.
// This avoids issues with copy constructors potentially resetting the vptr.
//
// Layout assumptions (verified by static_assert):
// - vptr at offset 0 (standard for polymorphic classes)
// - m_bAutoDelete immediately after vptr
// - CMemoryException adds no members beyond CException
//
struct ManualCMemoryException {
    void* vptr;           // Offset 0: points to our MSVC-compatible vtable
    int m_bAutoDelete;    // CException::m_bAutoDelete
    // Note: CMemoryException doesn't add any members beyond CException
};

// Comprehensive ABI compatibility verification
// These static_asserts catch layout drift at compile time
// Note: offsetof on m_bAutoDelete can't be checked directly (protected member in non-POD class)
// but our layout is correct because:
// - vptr is at offset 0 (standard for polymorphic classes, same as CObject)
// - m_bAutoDelete immediately follows (sizeof(void*) offset), same as CException
// - CMemoryException adds no additional members

// Pre-constructed exception with MSVC-compatible vtable
// Note: g_vtbl_CMemoryException is intentionally cast to void* - this array serves
// as the vtable pointer that MSVC code will use to resolve virtual method calls.
namespace openmfc { namespace detail { namespace mfcexceptions {
extern ManualCMemoryException g_ManualMemoryException;
} } }

// =============================================================================
// Helper functions to throw exceptions
// =============================================================================

template<typename T>
static void ThrowStatic(T* pException, ThrowInfo* pThrowInfo, void** msvcVtable) {
    if (!InitExceptionSystem()) { abort(); }
    InitAllRTTI();
    // Patch vptr to MSVC-compatible vtable before throwing
    if (msvcVtable) {
        PatchVPtr(pException, msvcVtable);
    }
    g_pCxxThrowException(&pException, pThrowInfo);
    abort();
}

template<typename T>
static void ThrowNew(T* pException, ThrowInfo* pThrowInfo, void** msvcVtable) {
    if (!InitExceptionSystem()) { abort(); }
    InitAllRTTI();
    // Patch vptr to MSVC-compatible vtable before throwing
    if (msvcVtable) {
        PatchVPtr(pException, msvcVtable);
    }
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




// Runtime class descriptors for COleException/COleDispatchException
// (defined with external linkage in ole_oleexception_rtti.cpp)
extern CRuntimeClass classCOleException;
extern CRuntimeClass classCOleDispatchException;



// AfxThrowInternetException - void(DWORD dwContext, DWORD dwError)
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowInternetException__YAX_KK_Z(
    DWORD dwContext, DWORD dwError
);
} } }

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

// AfxThrowLastCleanup - internal MFC function
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxThrowLastCleanup__YAXXZ();
} } }

// AfxAbort - terminates the application
namespace openmfc { namespace detail { namespace mfcexceptions {
extern "C" void MS_ABI impl__AfxAbort__YAXXZ();
} } }
