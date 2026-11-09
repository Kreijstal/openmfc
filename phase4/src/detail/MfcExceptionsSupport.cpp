#include "MfcExceptionsSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ();

// Implementations this unit calls that are defined with their own class.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();

// Symbol: ?AfxThrowFileException@@YAXHJPEB_W@Z
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(
    int cause, LONG lOsError, const wchar_t* lpszFileName
) {
    CFileException* pEx = new CFileException(cause, lOsError);
    if (lpszFileName) {
        pEx->m_strFileName = lpszFileName;
    }
    ThrowNew(pEx, &TI_CFileException, g_vtbl_CFileException);
}
namespace openmfc { namespace detail { namespace mfcexceptions {
void CopyErrorText(wchar_t* out, UINT maxLen, const wchar_t* text) {
    if (!out || maxLen == 0) return;
    const wchar_t* src = text ? text : L"";
    wcsncpy(out, src, maxLen - 1);
    out[maxLen - 1] = L'\0';
}
void EmitDiagnosticText(const wchar_t* text) {
    if (!text) return;
    OutputDebugStringW(text);
    fwprintf(stderr, L"%ls", text);
}
void CopyClassName(const CException* pThis, wchar_t* out, size_t outCount) {
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
const wchar_t* FileCauseText(int cause) {
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
const wchar_t* ArchiveCauseText(int cause) {
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
CxxThrowExceptionFunc g_pCxxThrowException = nullptr;
HMODULE g_hVCRuntime = nullptr;
uintptr_t g_imageBase = 0;
bool g_initialized = false;
uintptr_t GetOurImageBase() {
    HMODULE hModule;
    if (GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&GetOurImageBase,
            &hModule)) {
        return (uintptr_t)hModule;
    }
    return 0;
}
void* MS_ABI dummy_dtor(void* p) { return p; }
int MS_ABI dummy_eq(const void* pThis, const void* pOther) { return pThis == pOther; }
int MS_ABI dummy_ne(const void* pThis, const void* pOther) { return pThis != pOther; }
const char* MS_ABI dummy_name(void*) { return "dummy"; }
void* g_dummyTypeInfoVFTable[] = {
    (void*)dummy_dtor, (void*)dummy_eq, (void*)dummy_ne,
    nullptr, nullptr, (void*)dummy_name, nullptr,
};
const void* GetTypeInfoVFTable() {
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
bool InitExceptionSystem() {
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
TD_CException_t TD_CException = { nullptr, nullptr, ".PEAVCException@@" };
TD_CObject_t TD_CObject = { nullptr, nullptr, ".PEAVCObject@@" };
CatchableType CT_CException = { 1, 0, 0, -1, 0, 8, 0 };
CatchableType CT_CObject = { 1, 0, 0, -1, 0, 8, 0 };
TD_CFileException_t TD_CFileException = { nullptr, nullptr, ".PEAVCFileException@@" };
CatchableType CT_CFileException = { 1, 0, 0, -1, 0, 8, 0 };
CTA_CFileException_t CTA_CFileException = { 3, {0,0,0} };
ThrowInfo TI_CFileException = { 0, 0, 0, 0 };
TD_CArchiveException_t TD_CArchiveException = { nullptr, nullptr, ".PEAVCArchiveException@@" };
CatchableType CT_CArchiveException = { 1, 0, 0, -1, 0, 8, 0 };
CTA_CArchiveException_t CTA_CArchiveException = { 3, {0,0,0} };
ThrowInfo TI_CArchiveException = { 0, 0, 0, 0 };
TD_CInternetException_t TD_CInternetException = { nullptr, nullptr, ".PEAVCInternetException@@" };
CatchableType CT_CInternetException = { 1, 0, 0, -1, 0, 8, 0 };
CTA_CInternetException_t CTA_CInternetException = { 3, {0,0,0} };
ThrowInfo TI_CInternetException = { 0, 0, 0, 0 };
TD_CDBException_t TD_CDBException = { nullptr, nullptr, ".PEAVCDBException@@" };
CatchableType CT_CDBException = { 1, 0, 0, -1, 0, 8, 0 };
CTA_CDBException_t CTA_CDBException = { 3, {0,0,0} };
ThrowInfo TI_CDBException = { 0, 0, 0, 0 };
TD_CDaoException_t TD_CDaoException = { nullptr, nullptr, ".PEAVCDaoException@@" };
CatchableType CT_CDaoException = { 1, 0, 0, -1, 0, 8, 0 };
CTA_CDaoException_t CTA_CDaoException = { 3, {0,0,0} };
ThrowInfo TI_CDaoException = { 0, 0, 0, 0 };
bool g_rttiInitialized = false;
void InitAllRTTI() {
    if (g_rttiInitialized) return;
    if (!g_typeInfoVFT) g_typeInfoVFT = GetTypeInfoVFTable();
    g_imageBase = GetOurImageBase();

    // Base types
    TD_CException.pVFTable = g_typeInfoVFT;
    TD_CObject.pVFTable = g_typeInfoVFT;
    CT_CException.pType = TO_RVA(&TD_CException);
    CT_CObject.pType = TO_RVA(&TD_CObject);

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
extern "C" int MS_ABI vtbl_ReportErrorPad(void* pThis) {
    if (!pThis) return 0;

    const CException* pException = static_cast<const CException*>(pThis);
    wchar_t className[64];
    CopyClassName(pException, className, sizeof(className) / sizeof(className[0]));

    unsigned int helpContext = 0;
    wchar_t errorText[512];
    const int hasMessage = pException->CException::GetErrorMessage(
        errorText,
        static_cast<unsigned int>(sizeof(errorText) / sizeof(errorText[0])),
        &helpContext
    );

    wchar_t diag[640];
    if (hasMessage) {
        _snwprintf(
            diag,
            sizeof(diag) / sizeof(diag[0]),
            L"Exception::ReportError fallback for %ls (help=%u): %ls\n",
            className,
            helpContext,
            errorText
        );
    } else {
        _snwprintf(
            diag,
            sizeof(diag) / sizeof(diag[0]),
            L"Exception::ReportError fallback for %ls (help=%u)\n",
            className,
            helpContext
        );
    }
    diag[(sizeof(diag) / sizeof(diag[0])) - 1] = L'\0';
    EmitDiagnosticText(L"Exception::ReportError slot fallback\n");
    EmitDiagnosticText(diag);
    return 1;
}
extern "C" void* MS_ABI dtor_CFileException(CFileException* pThis) {
    if (pThis) {
        // Call the actual C++ destructor to clean up members (e.g., m_strFileName)
        pThis->~CFileException();
    }
    return pThis;
}
extern "C" void* MS_ABI dtor_CArchiveException(CArchiveException* pThis) {
    if (pThis) {
        pThis->~CArchiveException();
    }
    return pThis;
}
extern "C" void MS_ABI opdelete_shim(void* pThis) {
    ::operator delete(pThis);
}
extern "C" void MS_ABI vtbl_Serialize(CObject* pThis, CArchive* pArchive) {
    if (!pThis || !pArchive) {
        return;
    }
    pThis->CObject::Serialize(*pArchive);
}
extern "C" void MS_ABI vtbl_AssertValid(const CObject* pThis) {
    if (!pThis) {
        return;
    }

    if (pThis->IsKindOf(&CFileException::classCFileException)) {
        static_cast<const CFileException*>(pThis)->CFileException::AssertValid();
        return;
    }

    if (pThis->IsKindOf(&CArchiveException::classCArchiveException)) {
        static_cast<const CArchiveException*>(pThis)->CArchiveException::AssertValid();
        return;
    }

    const CException* pEx = static_cast<const CException*>(pThis);
    pEx->CException::AssertValid();
}
extern "C" void MS_ABI vtbl_Dump(const CObject* pThis) {
    if (!pThis) {
        return;
    }

    if (pThis->IsKindOf(&CFileException::classCFileException)) {
        static_cast<const CFileException*>(pThis)->CFileException::Dump();
        return;
    }

    if (pThis->IsKindOf(&CArchiveException::classCArchiveException)) {
        static_cast<const CArchiveException*>(pThis)->CArchiveException::Dump();
        return;
    }

    const CException* pEx = static_cast<const CException*>(pThis);
    pEx->CException::Dump();
}
extern "C" int MS_ABI vtbl_GetErrorMessage(
    const CException* pThis, wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext
) {
    if (!pThis) {
        if (pnHelpContext) *pnHelpContext = 0;
        if (lpszError && nMaxError > 0) lpszError[0] = L'\0';
        return 0;
    }
    return pThis->CException::GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}
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
void* g_vtbl_CFileException[] = {
    reinterpret_cast<void*>(vtbl_CFileException_GetRuntimeClass),  // [0] GetRuntimeClass
    reinterpret_cast<void*>(dtor_CFileException),                   // [1] destructor (calls ~CFileException)
    reinterpret_cast<void*>(vtbl_Serialize),                        // [2] Serialize
    reinterpret_cast<void*>(vtbl_CFileException_AssertValid),       // [3] AssertValid
    reinterpret_cast<void*>(vtbl_CFileException_Dump),              // [4] Dump
    reinterpret_cast<void*>(vtbl_CFileException_GetErrorMessage),   // [5] GetErrorMessage (GEM-first order)
    reinterpret_cast<void*>(vtbl_CFileException_GetErrorMessage),   // [6] GetErrorMessage (ReportError-first order)
    reinterpret_cast<void*>(vtbl_ReportErrorPad)                   // [7] pad
};
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
void* g_vtbl_CArchiveException[] = {
    reinterpret_cast<void*>(vtbl_CArchiveException_GetRuntimeClass),  // [0] GetRuntimeClass
    reinterpret_cast<void*>(dtor_CArchiveException),                   // [1] destructor (calls ~CArchiveException)
    reinterpret_cast<void*>(vtbl_Serialize),                           // [2] Serialize
    reinterpret_cast<void*>(vtbl_CArchiveException_AssertValid),       // [3] AssertValid
    reinterpret_cast<void*>(vtbl_CArchiveException_Dump),              // [4] Dump
    reinterpret_cast<void*>(vtbl_CArchiveException_GetErrorMessage),   // [5] GetErrorMessage (GEM-first order)
    reinterpret_cast<void*>(vtbl_CArchiveException_GetErrorMessage),   // [6] GetErrorMessage (ReportError-first order)
    reinterpret_cast<void*>(vtbl_ReportErrorPad)                        // [7] pad
};
// Symbol: ?AfxThrowArchiveException@@YAXHPEB_W@Z
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(
    int cause, const wchar_t* lpszArchiveName
) {
    CArchiveException* pEx = new CArchiveException(cause, lpszArchiveName);
    ThrowNew(pEx, &TI_CArchiveException, g_vtbl_CArchiveException);
}
// Symbol: ?AfxThrowInternetException@@YAX_KK@Z
extern "C" void MS_ABI impl__AfxThrowInternetException__YAX_KK_Z(
    DWORD dwContext, DWORD dwError
) {
    CInternetException* pEx = new CInternetException(dwError);
    pEx->m_dwContext = dwContext;
    ThrowNew(pEx, &TI_CInternetException, nullptr);
}
extern "C" void MS_ABI impl__AfxThrowDaoException__YAXHJ_Z(
    int nAfxDaoError, SCODE scode
) {
    CDaoException* pEx = new CDaoException();
    pEx->m_nAfxDaoError = nAfxDaoError;
    pEx->m_scode = scode;
    pEx->m_strDaoOrigin = CString(L"DAO exception");
    if (scode != 0) {
        pEx->m_strError.Format(L"DAO error (code=%ld, daoError=%ld)",
                               static_cast<long>(scode),
                               static_cast<long>(nAfxDaoError));
    } else {
        pEx->m_strError.Format(L"DAO error (daoError=%ld)",
                               static_cast<long>(nAfxDaoError));
    }
    ThrowNew(pEx, &TI_CDaoException, nullptr);
}
// Symbol: ?AfxThrowLastCleanup@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowLastCleanup__YAXXZ() {
    // This is typically called to throw a generic exception during cleanup
    impl__AfxThrowUserException__YAXXZ();
}
// Symbol: ?AfxAbort@@YAXXZ
extern "C" void MS_ABI impl__AfxAbort__YAXXZ() {
    fprintf(stderr, "AfxAbort: Terminating application\n");
    abort();
}
} } }  // namespace openmfc::detail::mfcexceptions
