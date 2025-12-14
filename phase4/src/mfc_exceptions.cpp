// MFC Exception Implementation for OpenMFC
//
// This implements all AfxThrowXxxException functions using MSVC-compatible
// C++ exception structures so that MSVC-compiled code can catch them.
//
// Supported exception types:
// - CMemoryException (Static instance)
// - CNotSupportedException
// - CResourceException
// - CUserException
// - CInvalidArgException
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

#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>

// Implement CException runtime class
IMPLEMENT_DYNAMIC(CException, CObject)

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Missing Exception Class Definitions
// =============================================================================

// CNotSupportedException
class CNotSupportedException : public CException {
public:
    CNotSupportedException() : CException(TRUE) {}
};

// CResourceException
class CResourceException : public CException {
public:
    CResourceException() : CException(TRUE) {}
};

// CUserException
class CUserException : public CException {
public:
    CUserException() : CException(TRUE) {}
};

// CInvalidArgException
class CInvalidArgException : public CException {
public:
    CInvalidArgException() : CException(TRUE) {}
};

// COleException
class COleException : public CException {
public:
    SCODE m_sc;
    static SCODE PASCAL Process(const CException* pAnyException);

    COleException() : CException(TRUE), m_sc(0) {}
};

// COleDispatchException
class COleDispatchException : public CException {
public:
    WORD m_wCode;
    CString m_strDescription;
    DWORD m_dwHelpContext;
    CString m_strSource;
    CString m_strHelpFile;

    COleDispatchException() : CException(TRUE), m_wCode(0), m_dwHelpContext(0) {}
};

// CInternetException
class CInternetException : public CException {
public:
    DWORD m_dwError;
    DWORD_PTR m_dwContext;

    CInternetException(DWORD dwError) : CException(TRUE), m_dwError(dwError), m_dwContext(0) {}
};

// CDBException
class CDBException : public CException {
public:
    short m_nRetCode;
    CString m_strError;
    CString m_strStateNativeOrigin;

    CDBException(short nRetCode) : CException(TRUE), m_nRetCode(nRetCode) {}
};

// CDaoException (Minimal definition)
class CDaoException : public CException {
public:
    SCODE m_scode;
    // Missing: CDaoErrorInfo* m_pErrorInfo;
    short m_nAfxDaoError;

    CDaoException() : CException(TRUE), m_scode(0), m_nAfxDaoError(0) {}
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

static CxxThrowExceptionFunc g_pCxxThrowException = nullptr;
static HMODULE g_hVCRuntime = nullptr;
static uintptr_t g_imageBase = 0;
static const void* g_typeInfoVFT = nullptr;
static bool g_initialized = false;

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

#define TO_RVA(addr) ((int32_t)((uintptr_t)(addr) - g_imageBase))

// Dummy type_info vftable
static void* MS_ABI dummy_dtor(void* p) { return p; }
static int MS_ABI dummy_eq(void*, void*) { return 0; }
static int MS_ABI dummy_ne(void*, void*) { return 1; }
static const char* MS_ABI dummy_name(void*) { return "dummy"; }

static void* g_dummyTypeInfoVFTable[] = {
    (void*)dummy_dtor, (void*)dummy_eq, (void*)dummy_ne,
    nullptr, nullptr, (void*)dummy_name, nullptr,
};

static const void* GetTypeInfoVFTable() {
    if (!g_hVCRuntime) g_hVCRuntime = LoadLibraryA("vcruntime140.dll");
    if (g_hVCRuntime) {
        void* vft = (void*)GetProcAddress(g_hVCRuntime, "??_7type_info@@6B@");
        if (vft) return vft;
    }
    HMODULE h = LoadLibraryA("ucrtbase.dll");
    if (h) {
        void* vft = (void*)GetProcAddress(h, "??_7type_info@@6B@");
        if (vft) return vft;
    }
    h = LoadLibraryA("msvcrt.dll");
    if (h) {
        void* vft = (void*)GetProcAddress(h, "??_7type_info@@6B@");
        if (vft) return vft;
    }
    return &g_dummyTypeInfoVFTable[0];
}

static bool InitExceptionSystem() {
    if (g_initialized) return true;

    g_imageBase = GetOurImageBase();
    if (!g_imageBase) return false;

    g_hVCRuntime = LoadLibraryA("vcruntime140.dll");
    if (!g_hVCRuntime) g_hVCRuntime = LoadLibraryA("msvcrt.dll");
    if (!g_hVCRuntime) return false;

    g_pCxxThrowException = (CxxThrowExceptionFunc)GetProcAddress(g_hVCRuntime, "_CxxThrowException");
    if (!g_pCxxThrowException) return false;

    g_typeInfoVFT = GetTypeInfoVFTable();
    g_initialized = true;
    return true;
}

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
static struct {
    const void* pVFTable;
    void* spare;
    char name[20];
} TD_CException = { nullptr, nullptr, ".PEAVCException@@" };

static struct {
    const void* pVFTable;
    void* spare;
    char name[16];
} TD_CObject = { nullptr, nullptr, ".PEAVCObject@@" };

static CatchableType CT_CException = { 1, 0, 0, -1, 0, 8, 0 };
static CatchableType CT_CObject = { 1, 0, 0, -1, 0, 8, 0 };

// Define all exception types
// CMemoryException
static struct {
    const void* pVFTable;
    void* spare;
    char name[28];
} TD_CMemoryException = { nullptr, nullptr, ".PEAVCMemoryException@@" };
static CatchableType CT_CMemoryException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CMemoryException = { 3, {0,0,0} };
static ThrowInfo TI_CMemoryException = { 0, 0, 0, 0 };
static CMemoryException g_MemoryException; // Static instance

// CNotSupportedException
static struct {
    const void* pVFTable;
    void* spare;
    char name[32];
} TD_CNotSupportedException = { nullptr, nullptr, ".PEAVCNotSupportedException@@" };
static CatchableType CT_CNotSupportedException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CNotSupportedException = { 3, {0,0,0} };
static ThrowInfo TI_CNotSupportedException = { 0, 0, 0, 0 };

// CResourceException
static struct {
    const void* pVFTable;
    void* spare;
    char name[28];
} TD_CResourceException = { nullptr, nullptr, ".PEAVCResourceException@@" };
static CatchableType CT_CResourceException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CResourceException = { 3, {0,0,0} };
static ThrowInfo TI_CResourceException = { 0, 0, 0, 0 };

// CUserException
static struct {
    const void* pVFTable;
    void* spare;
    char name[24];
} TD_CUserException = { nullptr, nullptr, ".PEAVCUserException@@" };
static CatchableType CT_CUserException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CUserException = { 3, {0,0,0} };
static ThrowInfo TI_CUserException = { 0, 0, 0, 0 };

// CInvalidArgException
static struct {
    const void* pVFTable;
    void* spare;
    char name[32];
} TD_CInvalidArgException = { nullptr, nullptr, ".PEAVCInvalidArgException@@" };
static CatchableType CT_CInvalidArgException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CInvalidArgException = { 3, {0,0,0} };
static ThrowInfo TI_CInvalidArgException = { 0, 0, 0, 0 };

// CFileException
static struct {
    const void* pVFTable;
    void* spare;
    char name[24];
} TD_CFileException = { nullptr, nullptr, ".PEAVCFileException@@" };
static CatchableType CT_CFileException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CFileException = { 3, {0,0,0} };
static ThrowInfo TI_CFileException = { 0, 0, 0, 0 };

// CArchiveException
static struct {
    const void* pVFTable;
    void* spare;
    char name[28];
} TD_CArchiveException = { nullptr, nullptr, ".PEAVCArchiveException@@" };
static CatchableType CT_CArchiveException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CArchiveException = { 3, {0,0,0} };
static ThrowInfo TI_CArchiveException = { 0, 0, 0, 0 };

// COleException
static struct {
    const void* pVFTable;
    void* spare;
    char name[24];
} TD_COleException = { nullptr, nullptr, ".PEAVCOleException@@" };
static CatchableType CT_COleException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_COleException = { 3, {0,0,0} };
static ThrowInfo TI_COleException = { 0, 0, 0, 0 };

// COleDispatchException
static struct {
    const void* pVFTable;
    void* spare;
    char name[32];
} TD_COleDispatchException = { nullptr, nullptr, ".PEAVCOleDispatchException@@" };
static CatchableType CT_COleDispatchException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[4]; } CTA_COleDispatchException = { 4, {0,0,0,0} };
static ThrowInfo TI_COleDispatchException = { 0, 0, 0, 0 };

// CInternetException
static struct {
    const void* pVFTable;
    void* spare;
    char name[28];
} TD_CInternetException = { nullptr, nullptr, ".PEAVCInternetException@@" };
static CatchableType CT_CInternetException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CInternetException = { 3, {0,0,0} };
static ThrowInfo TI_CInternetException = { 0, 0, 0, 0 };

// CDBException
static struct {
    const void* pVFTable;
    void* spare;
    char name[24];
} TD_CDBException = { nullptr, nullptr, ".PEAVCDBException@@" };
static CatchableType CT_CDBException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CDBException = { 3, {0,0,0} };
static ThrowInfo TI_CDBException = { 0, 0, 0, 0 };

// CDaoException
static struct {
    const void* pVFTable;
    void* spare;
    char name[25];
} TD_CDaoException = { nullptr, nullptr, ".PEAVCDaoException@@" };
static CatchableType CT_CDaoException = { 1, 0, 0, -1, 0, 8, 0 };
static struct { uint32_t n; int32_t t[3]; } CTA_CDaoException = { 3, {0,0,0} };
static ThrowInfo TI_CDaoException = { 0, 0, 0, 0 };

// =============================================================================
// Initialize RTTI for all exception types
// =============================================================================

static bool g_rttiInitialized = false;

static void InitAllRTTI() {
    if (g_rttiInitialized) return;
    if (!g_typeInfoVFT) g_typeInfoVFT = GetTypeInfoVFTable();
    g_imageBase = GetOurImageBase();

    // Base types
    TD_CException.pVFTable = g_typeInfoVFT;
    TD_CObject.pVFTable = g_typeInfoVFT;
    CT_CException.pType = TO_RVA(&TD_CException);
    CT_CObject.pType = TO_RVA(&TD_CObject);

    // CMemoryException
    TD_CMemoryException.pVFTable = g_typeInfoVFT;
    CT_CMemoryException.pType = TO_RVA(&TD_CMemoryException);
    CTA_CMemoryException.t[0] = TO_RVA(&CT_CMemoryException);
    CTA_CMemoryException.t[1] = TO_RVA(&CT_CException);
    CTA_CMemoryException.t[2] = TO_RVA(&CT_CObject);
    TI_CMemoryException.pmfnUnwind = 0; // No destructor needed for static object
    TI_CMemoryException.pCatchableTypeArray = TO_RVA(&CTA_CMemoryException);

    // CNotSupportedException
    TD_CNotSupportedException.pVFTable = g_typeInfoVFT;
    CT_CNotSupportedException.pType = TO_RVA(&TD_CNotSupportedException);
    CTA_CNotSupportedException.t[0] = TO_RVA(&CT_CNotSupportedException);
    CTA_CNotSupportedException.t[1] = TO_RVA(&CT_CException);
    CTA_CNotSupportedException.t[2] = TO_RVA(&CT_CObject);
    TI_CNotSupportedException.pmfnUnwind = 0; // We don't support unwinding on MinGW side easily
    TI_CNotSupportedException.pCatchableTypeArray = TO_RVA(&CTA_CNotSupportedException);

    // CResourceException
    TD_CResourceException.pVFTable = g_typeInfoVFT;
    CT_CResourceException.pType = TO_RVA(&TD_CResourceException);
    CTA_CResourceException.t[0] = TO_RVA(&CT_CResourceException);
    CTA_CResourceException.t[1] = TO_RVA(&CT_CException);
    CTA_CResourceException.t[2] = TO_RVA(&CT_CObject);
    TI_CResourceException.pmfnUnwind = 0;
    TI_CResourceException.pCatchableTypeArray = TO_RVA(&CTA_CResourceException);

    // CUserException
    TD_CUserException.pVFTable = g_typeInfoVFT;
    CT_CUserException.pType = TO_RVA(&TD_CUserException);
    CTA_CUserException.t[0] = TO_RVA(&CT_CUserException);
    CTA_CUserException.t[1] = TO_RVA(&CT_CException);
    CTA_CUserException.t[2] = TO_RVA(&CT_CObject);
    TI_CUserException.pmfnUnwind = 0;
    TI_CUserException.pCatchableTypeArray = TO_RVA(&CTA_CUserException);

    // CInvalidArgException
    TD_CInvalidArgException.pVFTable = g_typeInfoVFT;
    CT_CInvalidArgException.pType = TO_RVA(&TD_CInvalidArgException);
    CTA_CInvalidArgException.t[0] = TO_RVA(&CT_CInvalidArgException);
    CTA_CInvalidArgException.t[1] = TO_RVA(&CT_CException);
    CTA_CInvalidArgException.t[2] = TO_RVA(&CT_CObject);
    TI_CInvalidArgException.pmfnUnwind = 0;
    TI_CInvalidArgException.pCatchableTypeArray = TO_RVA(&CTA_CInvalidArgException);

    // CFileException
    TD_CFileException.pVFTable = g_typeInfoVFT;
    CT_CFileException.pType = TO_RVA(&TD_CFileException);
    CTA_CFileException.t[0] = TO_RVA(&CT_CFileException);
    CTA_CFileException.t[1] = TO_RVA(&CT_CException);
    CTA_CFileException.t[2] = TO_RVA(&CT_CObject);
    TI_CFileException.pmfnUnwind = 0;
    TI_CFileException.pCatchableTypeArray = TO_RVA(&CTA_CFileException);

    // CArchiveException
    TD_CArchiveException.pVFTable = g_typeInfoVFT;
    CT_CArchiveException.pType = TO_RVA(&TD_CArchiveException);
    CTA_CArchiveException.t[0] = TO_RVA(&CT_CArchiveException);
    CTA_CArchiveException.t[1] = TO_RVA(&CT_CException);
    CTA_CArchiveException.t[2] = TO_RVA(&CT_CObject);
    TI_CArchiveException.pmfnUnwind = 0;
    TI_CArchiveException.pCatchableTypeArray = TO_RVA(&CTA_CArchiveException);

    // COleException
    TD_COleException.pVFTable = g_typeInfoVFT;
    CT_COleException.pType = TO_RVA(&TD_COleException);
    CTA_COleException.t[0] = TO_RVA(&CT_COleException);
    CTA_COleException.t[1] = TO_RVA(&CT_CException);
    CTA_COleException.t[2] = TO_RVA(&CT_CObject);
    TI_COleException.pmfnUnwind = 0;
    TI_COleException.pCatchableTypeArray = TO_RVA(&CTA_COleException);

    // COleDispatchException
    TD_COleDispatchException.pVFTable = g_typeInfoVFT;
    CT_COleDispatchException.pType = TO_RVA(&TD_COleDispatchException);
    CTA_COleDispatchException.t[0] = TO_RVA(&CT_COleDispatchException);
    CTA_COleDispatchException.t[1] = TO_RVA(&CT_COleException);
    CTA_COleDispatchException.t[2] = TO_RVA(&CT_CException);
    CTA_COleDispatchException.t[3] = TO_RVA(&CT_CObject);
    TI_COleDispatchException.pmfnUnwind = 0;
    TI_COleDispatchException.pCatchableTypeArray = TO_RVA(&CTA_COleDispatchException);

    // CInternetException
    TD_CInternetException.pVFTable = g_typeInfoVFT;
    CT_CInternetException.pType = TO_RVA(&TD_CInternetException);
    CTA_CInternetException.t[0] = TO_RVA(&CT_CInternetException);
    CTA_CInternetException.t[1] = TO_RVA(&CT_CException);
    CTA_CInternetException.t[2] = TO_RVA(&CT_CObject);
    TI_CInternetException.pmfnUnwind = 0;
    TI_CInternetException.pCatchableTypeArray = TO_RVA(&CTA_CInternetException);

    // CDBException
    TD_CDBException.pVFTable = g_typeInfoVFT;
    CT_CDBException.pType = TO_RVA(&TD_CDBException);
    CTA_CDBException.t[0] = TO_RVA(&CT_CDBException);
    CTA_CDBException.t[1] = TO_RVA(&CT_CException);
    CTA_CDBException.t[2] = TO_RVA(&CT_CObject);
    TI_CDBException.pmfnUnwind = 0;
    TI_CDBException.pCatchableTypeArray = TO_RVA(&CTA_CDBException);

    // CDaoException
    TD_CDaoException.pVFTable = g_typeInfoVFT;
    CT_CDaoException.pType = TO_RVA(&TD_CDaoException);
    CTA_CDaoException.t[0] = TO_RVA(&CT_CDaoException);
    CTA_CDaoException.t[1] = TO_RVA(&CT_CException);
    CTA_CDaoException.t[2] = TO_RVA(&CT_CObject);
    TI_CDaoException.pmfnUnwind = 0;
    TI_CDaoException.pCatchableTypeArray = TO_RVA(&CTA_CDaoException);

    g_rttiInitialized = true;
}

// =============================================================================
// Helper functions to throw exceptions
// =============================================================================

template<typename T>
static void ThrowStatic(T* pException, ThrowInfo* pThrowInfo) {
    if (!InitExceptionSystem()) { abort(); }
    InitAllRTTI();
    g_pCxxThrowException(&pException, pThrowInfo);
    abort();
}

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
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    ThrowStatic(&g_MemoryException, &TI_CMemoryException);
}

// AfxThrowNotSupportedException - void()
extern "C" void MS_ABI stub__AfxThrowNotSupportedException__YAXXZ() {
    ThrowNew(new CNotSupportedException(), &TI_CNotSupportedException);
}

// AfxThrowResourceException - void()
extern "C" void MS_ABI stub__AfxThrowResourceException__YAXXZ() {
    ThrowNew(new CResourceException(), &TI_CResourceException);
}

// AfxThrowUserException - void()
extern "C" void MS_ABI stub__AfxThrowUserException__YAXXZ() {
    ThrowNew(new CUserException(), &TI_CUserException);
}

// AfxThrowInvalidArgException - void()
extern "C" void MS_ABI stub__AfxThrowInvalidArgException__YAXXZ() {
    ThrowNew(new CInvalidArgException(), &TI_CInvalidArgException);
}

// AfxThrowFileException - void(int cause, LONG lOsError, const wchar_t* lpszFileName)
extern "C" void MS_ABI stub__AfxThrowFileException__YAXHJPEB_W_Z(
    int cause, LONG lOsError, const wchar_t* lpszFileName
) {
    CFileException* pEx = new CFileException(cause, lOsError);
    if (lpszFileName) {
        pEx->m_strFileName = lpszFileName;
    }
    ThrowNew(pEx, &TI_CFileException);
}

// AfxThrowArchiveException - void(int cause, const wchar_t* lpszArchiveName)
extern "C" void MS_ABI stub__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName
) {
    CArchiveException* pEx = new CArchiveException(cause, lpszArchiveName);
    ThrowNew(pEx, &TI_CArchiveException);
}

// AfxThrowOleException - void(HRESULT sc)
extern "C" void MS_ABI stub__AfxThrowOleException__YAXJ_Z(LONG sc) {
    COleException* pEx = new COleException();
    pEx->m_sc = sc;
    ThrowNew(pEx, &TI_COleException);
}

// AfxThrowOleDispatchException - void(WORD wCode, UINT nDescriptionID, UINT nHelpID)
extern "C" void MS_ABI stub__AfxThrowOleDispatchException__YAXGII_Z(
    WORD wCode, UINT nDescriptionID, UINT nHelpID
) {
    COleDispatchException* pEx = new COleDispatchException();
    pEx->m_wCode = wCode;
    pEx->m_dwHelpContext = nHelpID;
    // TODO: Load description from resource nDescriptionID
    ThrowNew(pEx, &TI_COleDispatchException);
}

// AfxThrowOleDispatchException - void(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID)
extern "C" void MS_ABI stub__AfxThrowOleDispatchException__YAXGPEB_WI_Z(
    WORD wCode, const wchar_t* lpszDescription, UINT nHelpID
) {
    COleDispatchException* pEx = new COleDispatchException();
    pEx->m_wCode = wCode;
    pEx->m_strDescription = lpszDescription;
    pEx->m_dwHelpContext = nHelpID;
    ThrowNew(pEx, &TI_COleDispatchException);
}

// AfxThrowInternetException - void(DWORD dwContext, DWORD dwError)
// Symbol: ?AfxThrowInternetException@@YAX_KK@Z (x64: uses DWORD_PTR/_K)
extern "C" void MS_ABI stub__AfxThrowInternetException__YAX_KK_Z(
    DWORD dwContext, DWORD dwError
) {
    CInternetException* pEx = new CInternetException(dwError);
    pEx->m_dwContext = dwContext;
    ThrowNew(pEx, &TI_CInternetException);
}

// AfxThrowDBException - void(short nRetCode, CDatabase* pdb, void* hstmt)
extern "C" void MS_ABI stub__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(
    short nRetCode, void* pdb, void* hstmt
) {
    (void)pdb; (void)hstmt; // Unused for now
    CDBException* pEx = new CDBException(nRetCode);
    ThrowNew(pEx, &TI_CDBException);
}

// AfxThrowDaoException - void(int nAfxDaoError, SCODE scode)
// Note: Signature might vary, checking ordinal mapping would be ideal but assuming standard
extern "C" void MS_ABI stub__AfxThrowDaoException__YAXHJ_Z(
    int nAfxDaoError, SCODE scode
) {
    CDaoException* pEx = new CDaoException();
    pEx->m_nAfxDaoError = (short)nAfxDaoError;
    pEx->m_scode = scode;
    ThrowNew(pEx, &TI_CDaoException);
}

// AfxThrowLastCleanup - internal MFC function
extern "C" void MS_ABI stub__AfxThrowLastCleanup__YAXXZ() {
    // This is typically called to throw a generic exception during cleanup
    ThrowNew(new CUserException(), &TI_CUserException);
}

// AfxAbort - terminates the application
extern "C" void MS_ABI stub__AfxAbort__YAXXZ() {
    fprintf(stderr, "AfxAbort: Terminating application\n");
    abort();
}
