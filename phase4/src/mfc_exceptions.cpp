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
IMPLEMENT_DYNAMIC(CException, CObject)

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

namespace {

std::mutex g_oleDispatchDescriptionMutex;
std::unordered_map<const COleDispatchException*, std::wstring> g_oleDispatchDescriptions;

static void CopyErrorText(wchar_t* out, UINT maxLen, const wchar_t* text) {
    if (!out || maxLen == 0) return;
    const wchar_t* src = text ? text : L"";
    wcsncpy(out, src, maxLen - 1);
    out[maxLen - 1] = L'\0';
}

static void EmitDiagnosticText(const wchar_t* text) {
    if (!text) return;
    OutputDebugStringW(text);
    fwprintf(stderr, L"%ls", text);
}

static void CopyClassName(const CException* pThis, wchar_t* out, size_t outCount) {
    if (!out || outCount == 0) return;
    out[0] = L'\0';
    const char* name = "CException";
    if (pThis) {
        CRuntimeClass* pClass = pThis->GetRuntimeClass();
        if (pClass && pClass->m_lpszClassName && pClass->m_lpszClassName[0] != '\0') {
            name = pClass->m_lpszClassName;
        }
    }

    size_t i = 0;
    for (; name[i] != '\0' && i + 1 < outCount; ++i) {
        out[i] = static_cast<unsigned char>(name[i]);
    }
    out[i] = L'\0';
}

static const wchar_t* FileCauseText(int cause) {
    switch (cause) {
    case CFileException::none: return L"No error";
    case CFileException::genericException: return L"Generic file error";
    case CFileException::fileNotFound: return L"File not found";
    case CFileException::badPath: return L"Invalid path";
    case CFileException::tooManyOpenFiles: return L"Too many open files";
    case CFileException::accessDenied: return L"Access denied";
    case CFileException::invalidFile: return L"Invalid file";
    case CFileException::removeCurrentDir: return L"Cannot remove current directory";
    case CFileException::directoryFull: return L"Directory is full";
    case CFileException::badSeek: return L"Invalid seek operation";
    case CFileException::hardIO: return L"Hardware I/O error";
    case CFileException::sharingViolation: return L"File sharing violation";
    case CFileException::lockViolation: return L"File lock violation";
    case CFileException::diskFull: return L"Disk is full";
    case CFileException::endOfFile: return L"Unexpected end of file";
    default: return L"Unknown file error";
    }
}

static const wchar_t* ArchiveCauseText(int cause) {
    switch (cause) {
    case CArchiveException::none: return L"No error";
    case CArchiveException::generic: return L"Archive error";
    case CArchiveException::readOnly: return L"Cannot write to read-only archive";
    case CArchiveException::endOfFile: return L"Unexpected end of file";
    case CArchiveException::writeOnly: return L"Cannot read from write-only archive";
    case CArchiveException::badIndex: return L"Invalid object index";
    case CArchiveException::badClass: return L"Invalid class found in archive";
    case CArchiveException::badSchema: return L"Schema mismatch in archive";
    case CArchiveException::badFormat: return L"Bad archive format";
    default: return L"Unknown archive error";
    }
}

} // namespace

int CException::GetErrorMessage(wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext) const {
    if (pnHelpContext) {
        *pnHelpContext = 0;
    }
    if (lpszError && nMaxError > 0) {
        lpszError[0] = L'\0';
    }
    return 0;
}

void CException::Dump() const {
    wchar_t className[64];
    CopyClassName(this, className, sizeof(className) / sizeof(className[0]));
    wchar_t buffer[160];
    _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
               L"%ls[autoDelete=%d]\n", className, m_bAutoDelete);
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}

void CException::AssertValid() const {
    CRuntimeClass* pClass = GetRuntimeClass();
    if (!pClass || !pClass->m_lpszClassName || pClass->m_lpszClassName[0] == '\0') {
        EmitDiagnosticText(L"CException::AssertValid missing runtime class information\n");
    }
    if (m_bAutoDelete != FALSE && m_bAutoDelete != TRUE) {
        wchar_t buffer[96];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CException::AssertValid unusual m_bAutoDelete=%d\n", m_bAutoDelete);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}

void CFileException::Dump() const {
    wchar_t buffer[512];
    if (!m_strFileName.IsEmpty()) {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException[cause=%d, message=%ls, osError=%ld, file=%ls, autoDelete=%d]\n",
                   m_cause, FileCauseText(m_cause), m_lOsError, static_cast<const wchar_t*>(m_strFileName),
                   m_bAutoDelete);
    } else {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException[cause=%d, message=%ls, osError=%ld, autoDelete=%d]\n",
                   m_cause, FileCauseText(m_cause), m_lOsError, m_bAutoDelete);
    }
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}

void CFileException::AssertValid() const {
    CException::AssertValid();
    if (m_cause < none || m_cause > endOfFile) {
        wchar_t buffer[96];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException::AssertValid unknown cause=%d\n", m_cause);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}

void CArchiveException::Dump() const {
    wchar_t buffer[512];
    if (!m_strFileName.IsEmpty()) {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException[cause=%d, message=%ls, file=%ls, autoDelete=%d]\n",
                   m_cause, ArchiveCauseText(m_cause), static_cast<const wchar_t*>(m_strFileName), m_bAutoDelete);
    } else {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException[cause=%d, message=%ls, autoDelete=%d]\n",
                   m_cause, ArchiveCauseText(m_cause), m_bAutoDelete);
    }
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}

void CArchiveException::AssertValid() const {
    CException::AssertValid();
    if (m_cause < none || m_cause > badFormat) {
        wchar_t buffer[104];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException::AssertValid unknown cause=%d\n", m_cause);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}

// Symbol: ?Dump@CException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CException__UEBAXXZ(const CException* pThis) {
    if (pThis) {
        pThis->CException::Dump();
    }
}

// Symbol: ?AssertValid@CException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CException__UEBAXXZ(const CException* pThis) {
    if (pThis) {
        pThis->CException::AssertValid();
    }
}

// Symbol: ?Dump@CFileException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CFileException__UEBAXXZ(const CFileException* pThis) {
    if (pThis) {
        pThis->CFileException::Dump();
    }
}

// Symbol: ?AssertValid@CFileException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CFileException__UEBAXXZ(const CFileException* pThis) {
    if (pThis) {
        pThis->CFileException::AssertValid();
    }
}

// Symbol: ?Dump@CArchiveException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CArchiveException__UEBAXXZ(const CArchiveException* pThis) {
    if (pThis) {
        pThis->CArchiveException::Dump();
    }
}

// Symbol: ?AssertValid@CArchiveException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CArchiveException__UEBAXXZ(const CArchiveException* pThis) {
    if (pThis) {
        pThis->CArchiveException::AssertValid();
    }
}

// Symbol: ?GetErrorMessage@CException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z(
    const CException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!pThis) {
        if (pnHelpContext) *pnHelpContext = 0;
        if (lpszError && nMaxError > 0) lpszError[0] = L'\0';
        return 0;
    }
    return pThis->CException::GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

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

void AFXAPI AfxThrowOleException(LONG sc) {
    impl__AfxThrowOleException__YAXJ_Z(sc);
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, UINT nDescriptionID, UINT nHelpID) {
    impl__AfxThrowOleDispatchException__YAXGII_Z(wCode, nDescriptionID, nHelpID);
}

void AFXAPI AfxThrowOleDispatchException(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID) {
    impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(wCode, lpszDescription, nHelpID);
}

// =============================================================================
// CException::GetThisClass Implementation
// =============================================================================

// Symbol: ?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ() {
    return &CException::classCException;
}

asm(".globl \"?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ\"\n"
    ".set \"?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ\", impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ\n");

// Symbol: ?GetRuntimeClass@CException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CException__UEBAPEAUCRuntimeClass__XZ(const CException*) {
    return &CException::classCException;
}

// Symbol: ?Delete@CException@@QEAAXXZ
extern "C" void MS_ABI impl__Delete_CException__QEAAXXZ(CException* pThis) {
    if (pThis) pThis->Delete();
}

// Symbol: ?ReportError@CException@@UEAAHII@Z
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException* pThis, unsigned int type, unsigned int) {
    if (!pThis) return FALSE;
    wchar_t buffer[512] = {};
    pThis->GetErrorMessage(buffer, 512, nullptr);
    ::MessageBoxW(nullptr, buffer[0] ? buffer : L"An MFC exception occurred.", L"OpenMFC", type ? type : (MB_OK | MB_ICONERROR));
    return TRUE;
}

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
// - CResourceException, CUserException: Use static instances
// - These use stub_dtor_static which does nothing (safe, no cleanup needed)
//
// Heap-allocated exceptions (m_bAutoDelete=1, may be deleted by MSVC):
// - CFileException, CArchiveException: Created with 'new'
// - These use proper destructor shims (dtor_CFileException, dtor_CArchiveException)
// - The shims call the actual C++ destructor to clean up members
// - Memory deallocation uses MinGW's operator delete (via the same heap)
//
// Destructor stub for static exceptions (CMemoryException, etc.) - no cleanup needed
// Returns 'this' as MSVC destructors do; caller won't deallocate since m_bAutoDelete=0
extern "C" void* MS_ABI stub_dtor_static(void* pThis) { return pThis; }

// Padding for the slot past ReportError. The harvested CException-family vtable
// (phase1/harvest/vtable_slots.json) is exactly:
//   [0] GetRuntimeClass  [1] ~dtor  [2] Serialize  [3] AssertValid  [4] Dump
//   [5] GetErrorMessage (CException, non-const)
//   [6] GetErrorMessage (CSimpleException, const)   <- the one CMemoryException uses
//   [7] ReportError
// We serve the message thunk at both [5] and [6] (the two distinct GetErrorMessage
// virtuals) and treat [7] ReportError as a safe no-op (returns 0) since nothing
// in our surface invokes it.
extern "C" int MS_ABI stub_vtable_pad(void*) { return 0; }

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
extern "C" void* MS_ABI dtor_CFileException(CFileException* pThis) {
    if (pThis) {
        // Call the actual C++ destructor to clean up members (e.g., m_strFileName)
        pThis->~CFileException();
    }
    return pThis;
}

// CArchiveException destructor shim
extern "C" void* MS_ABI dtor_CArchiveException(CArchiveException* pThis) {
    if (pThis) {
        pThis->~CArchiveException();
    }
    return pThis;
}

// Operator delete shim - ensures MinGW heap is used for deallocation
// This is called after the destructor when MSVC code calls 'delete pException'
extern "C" void MS_ABI opdelete_shim(void* pThis) {
    ::operator delete(pThis);
}

// Serialize does nothing for exceptions
extern "C" void MS_ABI stub_Serialize(CObject*, CArchive*) {}

// No-ops by design. These vtable slots are invoked by MSVC code on objects
// whose vptr we patched to our MSVC-layout vtable. Dispatching into a C++ method
// that performs an internal virtual call (e.g. CException::AssertValid ->
// GetRuntimeClass) would resolve through the patched vtable using MinGW's
// Itanium slot indices, which don't line up with the MSVC layout -> crash.
// Release-mode MFC AssertValid/Dump are effectively no-ops, so this is correct.
extern "C" void MS_ABI stub_AssertValid(const CObject* pThis) { (void)pThis; }

extern "C" void MS_ABI stub_Dump(const CObject* pThis) { (void)pThis; }

extern "C" int MS_ABI stub_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
) {
    if (!pThis) {
        if (pnHelpContext) *pnHelpContext = 0;
        if (lpszError && nMaxError > 0) lpszError[0] = L'\0';
        return 0;
    }
    return pThis->CException::GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

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

extern "C" CRuntimeClass* MS_ABI vtbl_CMemoryException_GetRuntimeClass(const CObject* pThis) {
    (void)pThis;
    // Call our exported GetThisClass - this returns the exact address MSVC expects
    return impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
}

// CMemoryException has no per-instance state, so its message is constant.
// Real MFC reports AFX_IDP_OUT_OF_MEMORY ("Out of memory.").
extern "C" int MS_ABI vtbl_CMemoryException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
) {
    (void)pThis;
    if (pnHelpContext) *pnHelpContext = 0;
    if (!lpszError || nMaxError == 0) return 0;
    CopyErrorText(lpszError, nMaxError, L"Out of memory.");
    return 1;
}

// CMemoryException vtable - used by g_ManualMemoryException (static, never deleted)
static void* g_vtbl_CMemoryException[] = {
    reinterpret_cast<void*>(vtbl_CMemoryException_GetRuntimeClass),  // [0] GetRuntimeClass
    reinterpret_cast<void*>(stub_dtor_static),                        // [1] destructor (no-op, static instance)
    reinterpret_cast<void*>(stub_Serialize),                          // [2] Serialize
    reinterpret_cast<void*>(stub_AssertValid),                        // [3] AssertValid
    reinterpret_cast<void*>(stub_Dump),                               // [4] Dump
    reinterpret_cast<void*>(vtbl_CMemoryException_GetErrorMessage),   // [5] GetErrorMessage (GEM-first order)
    reinterpret_cast<void*>(vtbl_CMemoryException_GetErrorMessage),   // [6] GetErrorMessage (ReportError-first order)
    reinterpret_cast<void*>(stub_vtable_pad)                          // [7] pad
};

// CFileException vtable
extern "C" CRuntimeClass* MS_ABI vtbl_CFileException_GetRuntimeClass(const CObject* pThis) {
    (void)pThis;
    return &CFileException::classCFileException;
}

extern "C" void MS_ABI vtbl_CFileException_AssertValid(const CObject* pThis) {
    if (!pThis) return;
    static_cast<const CFileException*>(pThis)->CFileException::AssertValid();
}

extern "C" void MS_ABI vtbl_CFileException_Dump(const CObject* pThis) {
    if (!pThis) return;
    static_cast<const CFileException*>(pThis)->CFileException::Dump();
}

extern "C" int MS_ABI vtbl_CFileException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
) {
    if (pnHelpContext) *pnHelpContext = 0;
    if (!lpszError || nMaxError == 0) return 0;
    if (!pThis) { lpszError[0] = L'\0'; return 0; }
    const CFileException* pFile = static_cast<const CFileException*>(pThis);
    CopyErrorText(lpszError, nMaxError, FileCauseText(pFile->m_cause));
    return 1;
}

// CFileException vtable - heap-allocated, needs proper destructor
static void* g_vtbl_CFileException[] = {
    reinterpret_cast<void*>(vtbl_CFileException_GetRuntimeClass),  // [0] GetRuntimeClass
    reinterpret_cast<void*>(dtor_CFileException),                   // [1] destructor (calls ~CFileException)
    reinterpret_cast<void*>(stub_Serialize),                        // [2] Serialize
    reinterpret_cast<void*>(stub_AssertValid),                      // [3] AssertValid (no-op: avoids cross-ABI virtual dispatch)
    reinterpret_cast<void*>(stub_Dump),                             // [4] Dump (no-op: avoids cross-ABI virtual dispatch)
    reinterpret_cast<void*>(vtbl_CFileException_GetErrorMessage),   // [5] GetErrorMessage (GEM-first order)
    reinterpret_cast<void*>(vtbl_CFileException_GetErrorMessage),   // [6] GetErrorMessage (ReportError-first order)
    reinterpret_cast<void*>(stub_vtable_pad)                        // [7] pad
};

// CArchiveException vtable
extern "C" CRuntimeClass* MS_ABI vtbl_CArchiveException_GetRuntimeClass(const CObject* pThis) {
    (void)pThis;
    return &CArchiveException::classCArchiveException;
}

extern "C" void MS_ABI vtbl_CArchiveException_AssertValid(const CObject* pThis) {
    if (!pThis) return;
    static_cast<const CArchiveException*>(pThis)->CArchiveException::AssertValid();
}

extern "C" void MS_ABI vtbl_CArchiveException_Dump(const CObject* pThis) {
    if (!pThis) return;
    static_cast<const CArchiveException*>(pThis)->CArchiveException::Dump();
}

extern "C" int MS_ABI vtbl_CArchiveException_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
) {
    if (pnHelpContext) *pnHelpContext = 0;
    if (!lpszError || nMaxError == 0) return 0;
    if (!pThis) { lpszError[0] = L'\0'; return 0; }
    const CArchiveException* pArc = static_cast<const CArchiveException*>(pThis);
    CopyErrorText(lpszError, nMaxError, ArchiveCauseText(pArc->m_cause));
    return 1;
}

// CArchiveException vtable - heap-allocated, needs proper destructor
static void* g_vtbl_CArchiveException[] = {
    reinterpret_cast<void*>(vtbl_CArchiveException_GetRuntimeClass),  // [0] GetRuntimeClass
    reinterpret_cast<void*>(dtor_CArchiveException),                   // [1] destructor (calls ~CArchiveException)
    reinterpret_cast<void*>(stub_Serialize),                           // [2] Serialize
    reinterpret_cast<void*>(stub_AssertValid),                         // [3] AssertValid (no-op: avoids cross-ABI virtual dispatch)
    reinterpret_cast<void*>(stub_Dump),                                // [4] Dump (no-op: avoids cross-ABI virtual dispatch)
    reinterpret_cast<void*>(vtbl_CArchiveException_GetErrorMessage),   // [5] GetErrorMessage (GEM-first order)
    reinterpret_cast<void*>(vtbl_CArchiveException_GetErrorMessage),   // [6] GetErrorMessage (ReportError-first order)
    reinterpret_cast<void*>(stub_vtable_pad)                           // [7] pad
};

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
static_assert(sizeof(ManualCMemoryException) == sizeof(CMemoryException),
              "ManualCMemoryException must match CMemoryException size for ABI compatibility");
static_assert(alignof(ManualCMemoryException) == alignof(CMemoryException),
              "ManualCMemoryException must match CMemoryException alignment for ABI compatibility");
// Note: offsetof on m_bAutoDelete can't be checked directly (protected member in non-POD class)
// but our layout is correct because:
// - vptr is at offset 0 (standard for polymorphic classes, same as CObject)
// - m_bAutoDelete immediately follows (sizeof(void*) offset), same as CException
// - CMemoryException adds no additional members
static_assert(offsetof(ManualCMemoryException, vptr) == 0,
              "ManualCMemoryException vptr must be at offset 0");
static_assert(offsetof(ManualCMemoryException, m_bAutoDelete) == sizeof(void*),
              "ManualCMemoryException m_bAutoDelete must immediately follow vptr");

// Pre-constructed exception with MSVC-compatible vtable
// Note: g_vtbl_CMemoryException is intentionally cast to void* - this array serves
// as the vtable pointer that MSVC code will use to resolve virtual method calls.
static ManualCMemoryException g_ManualMemoryException = {
    static_cast<void*>(g_vtbl_CMemoryException),  // vptr: explicit cast to MSVC vtable
    0                                              // m_bAutoDelete = 0 (static, not auto-deleted)
};

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
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ() {
    // Use the manually constructed exception with pre-set MSVC vtable
    // This avoids any issues with MinGW vtable layout
    CMemoryException* pEx = reinterpret_cast<CMemoryException*>(&g_ManualMemoryException);
    ThrowStatic(pEx, &TI_CMemoryException, nullptr);  // vtable already set
}

// AfxThrowNotSupportedException - void()
extern "C" void MS_ABI impl__AfxThrowNotSupportedException__YAXXZ() {
    ThrowNew(new CNotSupportedException(), &TI_CNotSupportedException, nullptr);
}

// AfxThrowResourceException - void()
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ() {
    ThrowNew(new CResourceException(), &TI_CResourceException, nullptr);
}

// AfxThrowUserException - void()
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ() {
    ThrowNew(new CUserException(), &TI_CUserException, nullptr);
}

// AfxThrowInvalidArgException - void()
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ() {
    ThrowNew(new CInvalidArgException(), &TI_CInvalidArgException, nullptr);
}

// AfxThrowFileException - void(int cause, LONG lOsError, const wchar_t* lpszFileName)
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(
    int cause, LONG lOsError, const wchar_t* lpszFileName
) {
    CFileException* pEx = new CFileException(cause, lOsError);
    if (lpszFileName) {
        pEx->m_strFileName = lpszFileName;
    }
    ThrowNew(pEx, &TI_CFileException, g_vtbl_CFileException);
}

// AfxThrowArchiveException - void(int cause, const wchar_t* lpszArchiveName)
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName
) {
    CArchiveException* pEx = new CArchiveException(cause, lpszArchiveName);
    ThrowNew(pEx, &TI_CArchiveException, g_vtbl_CArchiveException);
}

// AfxThrowOleException - void(HRESULT sc)
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc) {
    COleException* pEx = new COleException();
    pEx->m_sc = sc;
    ThrowNew(pEx, &TI_COleException, nullptr);
}

// AfxThrowOleDispatchException - void(WORD wCode, UINT nDescriptionID, UINT nHelpID)
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGII_Z(
    WORD wCode, UINT nDescriptionID, UINT nHelpID
) {
    COleDispatchException* pEx = new COleDispatchException();
    pEx->m_wCode = wCode;
    pEx->m_dwHelpContext = nHelpID;
    // TODO: Load description from resource nDescriptionID
    ThrowNew(pEx, &TI_COleDispatchException, nullptr);
}

// AfxThrowOleDispatchException - void(WORD wCode, const wchar_t* lpszDescription, UINT nHelpID)
extern "C" void MS_ABI impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(
    WORD wCode, const wchar_t* lpszDescription, UINT nHelpID
) {
    COleDispatchException* pEx = new COleDispatchException();
    pEx->m_wCode = wCode;
    pEx->m_dwHelpContext = nHelpID;
    {
        std::lock_guard<std::mutex> lock(g_oleDispatchDescriptionMutex);
        g_oleDispatchDescriptions[pEx] = lpszDescription ? lpszDescription : L"";
        pEx->m_strDescription = g_oleDispatchDescriptions[pEx].c_str();
    }
    ThrowNew(pEx, &TI_COleDispatchException, nullptr);
}

// Symbol: ??1COleDispatchException@@UEAA@XZ
extern "C" COleDispatchException* MS_ABI impl___1COleDispatchException__UEAA_XZ(
    COleDispatchException* pThis
) {
    if (!pThis) return nullptr;
    {
        std::lock_guard<std::mutex> lock(g_oleDispatchDescriptionMutex);
        g_oleDispatchDescriptions.erase(pThis);
    }
    pThis->~COleDispatchException();
    return pThis;
}

// Symbol: ?GetErrorMessage@COleException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z(
    const COleException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!lpszError || nMaxError == 0) return 0;
    if (pnHelpContext) *pnHelpContext = 0;
    if (!pThis) {
        lpszError[0] = L'\0';
        return 0;
    }

    const DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    DWORD len = FormatMessageW(
        flags, nullptr, static_cast<DWORD>(pThis->m_sc), 0, lpszError, nMaxError, nullptr
    );
    if (len != 0) {
        while (len > 0 && (lpszError[len - 1] == L'\r' || lpszError[len - 1] == L'\n')) {
            lpszError[len - 1] = L'\0';
            --len;
        }
        return 1;
    }

    swprintf(lpszError, nMaxError, L"OLE exception 0x%08lX", static_cast<unsigned long>(pThis->m_sc));
    return 1;
}

// Symbol: ?GetErrorMessage@COleDispatchException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z(
    const COleDispatchException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!lpszError || nMaxError == 0) return 0;
    if (pnHelpContext) *pnHelpContext = pThis ? pThis->m_dwHelpContext : 0;
    if (!pThis) {
        lpszError[0] = L'\0';
        return 0;
    }

    std::wstring desc;
    {
        std::lock_guard<std::mutex> lock(g_oleDispatchDescriptionMutex);
        auto it = g_oleDispatchDescriptions.find(pThis);
        if (it != g_oleDispatchDescriptions.end()) {
            desc = it->second;
        }
    }
    if (desc.empty() && pThis->m_strDescription) {
        desc = pThis->m_strDescription;
    }
    if (desc.empty()) {
        swprintf(lpszError, nMaxError, L"OLE dispatch exception (code %u)", pThis->m_wCode);
        return 1;
    }
    CopyErrorText(lpszError, nMaxError, desc.c_str());
    return 1;
}

// AfxThrowInternetException - void(DWORD dwContext, DWORD dwError)
extern "C" void MS_ABI impl__AfxThrowInternetException__YAX_KK_Z(
    DWORD dwContext, DWORD dwError
) {
    CInternetException* pEx = new CInternetException(dwError);
    pEx->m_dwContext = dwContext;
    ThrowNew(pEx, &TI_CInternetException, nullptr);
}

// AfxThrowDBException - void(short nRetCode, CDatabase* pdb, void* hstmt)
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(
    short nRetCode, void* pdb, void* hstmt
) {
    (void)pdb; (void)hstmt; // Unused for now
    CDBException* pEx = new CDBException(nRetCode);
    ThrowNew(pEx, &TI_CDBException, nullptr);
}

// AfxThrowDaoException - void(int nAfxDaoError, SCODE scode)
// Note: Signature might vary, checking ordinal mapping would be ideal but assuming standard
extern "C" void MS_ABI impl__AfxThrowDaoException__YAXHJ_Z(
    int nAfxDaoError, SCODE scode
) {
    CDaoException* pEx = new CDaoException();
    pEx->m_nAfxDaoError = (short)nAfxDaoError;
    pEx->m_scode = scode;
    ThrowNew(pEx, &TI_CDaoException, nullptr);
}

// AfxThrowLastCleanup - internal MFC function
extern "C" void MS_ABI impl__AfxThrowLastCleanup__YAXXZ() {
    // This is typically called to throw a generic exception during cleanup
    ThrowNew(new CUserException(), &TI_CUserException, nullptr);
}

// AfxAbort - terminates the application
extern "C" void MS_ABI impl__AfxAbort__YAXXZ() {
    fprintf(stderr, "AfxAbort: Terminating application\n");
    abort();
}
