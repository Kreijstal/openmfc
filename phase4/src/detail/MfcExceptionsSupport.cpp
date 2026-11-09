#include "MfcExceptionsSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ();

// Implementations this unit calls that are defined with their own class.
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();

namespace openmfc { namespace detail { namespace mfcexceptions {
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
// Symbol: ?AfxThrowInternetException@@YAX_KK@Z
extern "C" void MS_ABI impl__AfxThrowInternetException__YAX_KK_Z(
    DWORD dwContext, DWORD dwError
) {
    CInternetException* pEx = new CInternetException(dwError);
    pEx->m_dwContext = dwContext;
    ThrowNew(pEx, &TI_CInternetException);
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
    ThrowNew(pEx, &TI_CDaoException);
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
