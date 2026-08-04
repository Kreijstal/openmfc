// CInternetException — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ?GetRuntimeClass@CInternetException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInternetException__UEBAPEAUCRuntimeClass__XZ(
    const CInternetException* pThis) {
    return CInternetException::GetThisClass();
}
// Symbol: ?GetThisClass@CInternetException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInternetException__SAPEAUCRuntimeClass__XZ() {
    return CInternetException::GetThisClass();
}
CInternetException::CInternetException(DWORD dwError)
    : m_dwError(dwError), m_dwContext(0)
{
    memset(_inetex_padding, 0, sizeof(_inetex_padding));
}
CInternetException::~CInternetException() {
}
int CInternetException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError,
                                         UINT* pnHelpContext) const {
    if (pnHelpContext) *pnHelpContext = 0;
    if (!lpszError || nMaxError == 0) return FALSE;

    // Try to get system error message first
    DWORD dwLen = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, m_dwError, 0, lpszError, nMaxError, nullptr);
    if (dwLen == 0) {
        // Try WinInet error
        dwLen = FormatMessageW(
            FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
            GetModuleHandleW(L"wininet.dll"), m_dwError, 0,
            lpszError, nMaxError, nullptr);
    }
    return dwLen > 0;
}
int CInternetException::GetErrorMessage(char* lpszError, UINT nMaxError,
                                         UINT* pnHelpContext) const {
    if (pnHelpContext) *pnHelpContext = 0;
    if (!lpszError || nMaxError == 0) return FALSE;

    wchar_t wbuf[1024];
    int result = GetErrorMessage(wbuf, 1024, nullptr);
    if (result) {
        WideCharToMultiByte(CP_ACP, 0, wbuf, -1, lpszError, nMaxError, nullptr, nullptr);
    }
    return result;
}
