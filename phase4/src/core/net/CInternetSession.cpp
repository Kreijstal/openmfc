// CInternetSession — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ?GetRuntimeClass@CInternetSession@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInternetSession__UEBAPEAUCRuntimeClass__XZ(
    const CInternetSession* pThis) {
    return CInternetSession::GetThisClass();
}
// Symbol: ?GetThisClass@CInternetSession@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInternetSession__SAPEAUCRuntimeClass__XZ() {
    return CInternetSession::GetThisClass();
}
// Symbol: ?OnStatusCallback@CInternetSession@@UEAAX_KKPEAXK@Z
extern "C" void MS_ABI impl__OnStatusCallback_CInternetSession__UEAAX_KKPEAXK_Z(
    CInternetSession* pThis, unsigned __int64 dwContext, unsigned long dwInternetStatus,
    void* lpvStatusInformation, unsigned long dwStatusInformationLength) {
    pThis->OnStatusCallback((DWORD_PTR)dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}
// Symbol: ?GetCookieLength@CInternetSession@@SAKPEB_W0@Z
extern "C" unsigned long MS_ABI impl__GetCookieLength_CInternetSession__SAKPEB_W0_Z(
    const wchar_t* pstrUrl, const wchar_t* pstrCookieName) {
    return CInternetSession::GetCookieLength(pstrUrl, pstrCookieName);
}
// Symbol: ?GetCookie@CInternetSession@@SAHPEB_W0PEA_WK@Z
extern "C" int MS_ABI impl__GetCookie_CInternetSession__SAHPEB_W0PEA_WK_Z(
    const wchar_t* pstrUrl, const wchar_t* pstrCookieName, wchar_t* pstrCookieData, unsigned long dwLen) {
    return CInternetSession::GetCookie(pstrUrl, pstrCookieName, pstrCookieData, dwLen);
}
// Symbol: ?GetCookie@CInternetSession@@SAHPEB_W0AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetCookie_CInternetSession__SAHPEB_W0AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const wchar_t* pstrUrl, const wchar_t* pstrCookieName, CString& strCookieData) {
    return CInternetSession::GetCookie(pstrUrl, pstrCookieName, strCookieData);
}
// Symbol: ?SetCookie@CInternetSession@@SAHPEB_W00@Z
extern "C" int MS_ABI impl__SetCookie_CInternetSession__SAHPEB_W00_Z(
    const wchar_t* pstrUrl, const wchar_t* pstrCookieName, const wchar_t* pstrCookieData) {
    return CInternetSession::SetCookie(pstrUrl, pstrCookieName, pstrCookieData);
}
CInternetSession::CInternetSession(const wchar_t* pstrAgent,
                                    DWORD_PTR dwContext,
                                    DWORD dwAccessType,
                                    const wchar_t* pstrProxyName,
                                    const wchar_t* pstrProxyBypass,
                                    DWORD dwFlags)
    : m_hSession(nullptr), m_dwContext(dwContext), m_dwAccessType(dwAccessType),
      m_strAgent(pstrAgent ? pstrAgent : L"OpenMFC"), m_bCallbackEnabled(FALSE)
{
    memset(_inetsession_padding, 0, sizeof(_inetsession_padding));

    m_hSession = InternetOpenW(pstrAgent ? pstrAgent : L"OpenMFC",
                                dwAccessType,
                                pstrProxyName,
                                pstrProxyBypass,
                                dwFlags);
    if (m_hSession) {
        g_sessionMap[m_hSession] = this;
    }
}
CInternetSession::~CInternetSession() {
    Close();
}
void CInternetSession::Close() {
    if (m_hSession) {
        g_sessionMap.erase(m_hSession);
        InternetCloseHandle(m_hSession);
        m_hSession = nullptr;
    }
}
CHttpConnection* CInternetSession::GetHttpConnection(const wchar_t* pstrServer,
                                                       INTERNET_PORT nPort,
                                                       const wchar_t* pstrUserName,
                                                       const wchar_t* pstrPassword) {
    return new CHttpConnection(this, pstrServer, nPort, m_dwContext);
}
CHttpConnection* CInternetSession::GetHttpConnection(const wchar_t* pstrServer,
                                                       DWORD dwFlags,
                                                       INTERNET_PORT nPort,
                                                       const wchar_t* pstrUserName,
                                                       const wchar_t* pstrPassword) {
    return new CHttpConnection(this, pstrServer, dwFlags, nPort, m_dwContext);
}
CFtpConnection* CInternetSession::GetFtpConnection(const wchar_t* pstrServer,
                                                     const wchar_t* pstrUserName,
                                                     const wchar_t* pstrPassword,
                                                     INTERNET_PORT nPort,
                                                     int bPassive) {
    return new CFtpConnection(this, pstrServer, pstrUserName, pstrPassword,
                               m_dwContext, nPort, bPassive);
}
int CInternetSession::QueryOption(DWORD dwOption, void* lpBuffer, DWORD* pdwBufLen) const {
    if (!m_hSession) return FALSE;
    return InternetQueryOptionW(m_hSession, dwOption, lpBuffer, pdwBufLen);
}
int CInternetSession::SetOption(DWORD dwOption, void* lpBuffer, DWORD dwBufLen) {
    if (!m_hSession) return FALSE;
    return InternetSetOptionW(m_hSession, dwOption, lpBuffer, dwBufLen);
}
int CInternetSession::EnableStatusCallback(int bEnable) {
    if (!m_hSession) return FALSE;

    SetLastError(0);
    INTERNET_STATUS_CALLBACK prev;
    if (bEnable) {
        prev = InternetSetStatusCallbackW(m_hSession, &WinInetStatusCallback);
    } else {
        prev = InternetSetStatusCallbackW(m_hSession, INTERNET_INVALID_STATUS_CALLBACK);
    }

    if (prev == INTERNET_INVALID_STATUS_CALLBACK && GetLastError() != 0) {
        return FALSE;
    }
    m_bCallbackEnabled = bEnable;
    return TRUE;
}
CStdioFile* CInternetSession::OpenURL(const wchar_t* pstrURL, DWORD_PTR dwContext,
                                       DWORD dwFlags, const wchar_t* pstrHeaders,
                                       DWORD dwHeadersLength) {
    HINTERNET hUrl = ::InternetOpenUrlW(m_hSession, pstrURL, pstrHeaders,
                                         dwHeadersLength, dwFlags, (DWORD_PTR)dwContext);
    if (!hUrl) return nullptr;
    CHttpFile* pFile = new CHttpFile(hUrl, nullptr, L"GET", pstrURL, nullptr);
    return pFile;
}
DWORD CInternetSession::GetServiceType() const {
    return m_dwAccessType;
}
int CInternetSession::GetServiceTypeFromHandle(HINTERNET hConnect) {
    if (!hConnect) return 0;
    DWORD handleType = 0;
    DWORD cb = sizeof(handleType);
    if (!::InternetQueryOptionW(hConnect, INTERNET_OPTION_HANDLE_TYPE, &handleType, &cb)) {
        return 0;
    }
    switch (handleType) {
    case INTERNET_HANDLE_TYPE_CONNECT_HTTP:
    case INTERNET_HANDLE_TYPE_HTTP_REQUEST:
        return INTERNET_SERVICE_HTTP;
    case INTERNET_HANDLE_TYPE_CONNECT_FTP:
    case INTERNET_HANDLE_TYPE_FTP_FILE:
    case INTERNET_HANDLE_TYPE_FTP_FIND:
        return INTERNET_SERVICE_FTP;
    case INTERNET_HANDLE_TYPE_CONNECT_GOPHER:
        return INTERNET_SERVICE_GOPHER;
    default:
        return 0;
    }
}
int CInternetSession::GetFtpConnection(const wchar_t* pstrServer, 
                                        const wchar_t* pstrUserName,
                                        const wchar_t* pstrPassword,
                                        INTERNET_PORT nPort, int bPassive,
                                        CFtpConnection*& refConnection) {
    CInternetSession* pSession = SessionForStaticHelpers();
    refConnection = nullptr;
    if (!pSession) return 0;
    CFtpConnection* pConnection = pSession->GetFtpConnection(
        pstrServer, pstrUserName, pstrPassword, nPort, bPassive);
    if (!pConnection || !pConnection->m_hConnection) {
        delete pConnection;
        return 0;
    }
    refConnection = pConnection;
    return 1;
}
int CInternetSession::GetHttpConnection(const wchar_t* pstrServer, INTERNET_PORT nPort,
                                         const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                         CHttpConnection*& refConnection) {
    CInternetSession* pSession = SessionForStaticHelpers();
    refConnection = nullptr;
    if (!pSession) return 0;
    CHttpConnection* pConnection = pSession->GetHttpConnection(
        pstrServer, nPort, pstrUserName, pstrPassword);
    if (!pConnection || !pConnection->m_hConnection) {
        delete pConnection;
        return 0;
    }
    refConnection = pConnection;
    return 1;
}
int CInternetSession::GetHttpConnection(const wchar_t* pstrServer, DWORD dwFlags,
                                         INTERNET_PORT nPort,
                                         const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                         CHttpConnection*& refConnection) {
    CInternetSession* pSession = SessionForStaticHelpers();
    refConnection = nullptr;
    if (!pSession) return 0;
    CHttpConnection* pConnection = pSession->GetHttpConnection(
        pstrServer, dwFlags, nPort, pstrUserName, pstrPassword);
    if (!pConnection || !pConnection->m_hConnection) {
        delete pConnection;
        return 0;
    }
    refConnection = pConnection;
    return 1;
}
DWORD CInternetSession::GetCookieLength(const wchar_t* pstrUrl, const wchar_t* pstrCookieName) {
    DWORD dwLen = 0;
    InternetGetCookieW(pstrUrl, pstrCookieName, nullptr, &dwLen);
    return dwLen;
}
int CInternetSession::GetCookie(const wchar_t* pstrUrl, const wchar_t* pstrCookieName,
                                 wchar_t* pstrCookieData, DWORD dwLen) {
    DWORD actual = dwLen;
    return InternetGetCookieW(pstrUrl, pstrCookieName, pstrCookieData, &actual) ? TRUE : FALSE;
}
int CInternetSession::GetCookie(const wchar_t* pstrUrl, const wchar_t* pstrCookieName,
                                 CString& strCookieData) {
    DWORD dwLen = GetCookieLength(pstrUrl, pstrCookieName);
    if (dwLen == 0) {
        strCookieData.Empty();
        return FALSE;
    }
    std::vector<wchar_t> buf(dwLen, L'\0');
    int ok = GetCookie(pstrUrl, pstrCookieName, buf.data(), dwLen);
    if (ok) {
        strCookieData = buf.data();
    } else {
        strCookieData.Empty();
    }
    return ok;
}
int CInternetSession::SetCookie(const wchar_t* pstrUrl, const wchar_t* pstrCookieName,
                                 const wchar_t* pstrCookieData) {
    return InternetSetCookieW(pstrUrl, pstrCookieName, pstrCookieData) ? TRUE : FALSE;
}
void CInternetSession::OnStatusCallback(DWORD_PTR dwContext, DWORD dwInternetStatus,
                                        void* lpvStatusInformation, DWORD dwStatusInformationLength) {
    (void)dwContext;
    (void)dwInternetStatus;
    (void)lpvStatusInformation;
    (void)dwStatusInformationLength;
}
CGopherConnection* CInternetSession::GetGopherConnection(const wchar_t* pstrServer,
                                                          const wchar_t* pstrUserName,
                                                          const wchar_t* pstrPassword,
                                                          INTERNET_PORT nPort) {
    return new CGopherConnection(this, pstrServer, pstrUserName, pstrPassword, m_dwContext, nPort);
}
