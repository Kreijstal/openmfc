// CHttpConnection — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ??0CHttpConnection@@QEAA@PEAVCInternetSession@@PEAXPEB_W_K@Z
extern "C" void* MS_ABI impl___0CHttpConnection__QEAA_PEAVCInternetSession__PEAXPEB_W_K_Z(
    void* pThis, CInternetSession* pSession, void* hConnected, const wchar_t* pstrServer, unsigned __int64 dwContext) {
    return new (pThis) CHttpConnection(pSession, (HINTERNET)hConnected, pstrServer, (DWORD_PTR)dwContext);
}
// Symbol: ??0CHttpConnection@@QEAA@PEAVCInternetSession@@PEB_WG11_K@Z
extern "C" void* MS_ABI impl___0CHttpConnection__QEAA_PEAVCInternetSession__PEB_WG11_K_Z(
    void* pThis, CInternetSession* pSession, const wchar_t* pstrServer,
    unsigned short nPort, const wchar_t* pstrUserName, const wchar_t* pstrPassword,
    unsigned __int64 dwContext) {
    return new (pThis) CHttpConnection(pSession, pstrServer, (INTERNET_PORT)nPort,
                                       pstrUserName, pstrPassword, (DWORD_PTR)dwContext);
}
// Symbol: ??0CHttpConnection@@QEAA@PEAVCInternetSession@@PEB_WKG11_K@Z
extern "C" void* MS_ABI impl___0CHttpConnection__QEAA_PEAVCInternetSession__PEB_WKG11_K_Z(
    void* pThis, CInternetSession* pSession, const wchar_t* pstrServer, unsigned long dwFlags,
    unsigned short nPort, const wchar_t* pstrUserName, const wchar_t* pstrPassword,
    unsigned __int64 dwContext) {
    return new (pThis) CHttpConnection(pSession, pstrServer, dwFlags, (INTERNET_PORT)nPort,
                                       pstrUserName, pstrPassword, (DWORD_PTR)dwContext);
}
// Symbol: ?GetRuntimeClass@CHttpConnection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHttpConnection__UEBAPEAUCRuntimeClass__XZ(
    const CHttpConnection* pThis) {
    return CHttpConnection::GetThisClass();
}
// Symbol: ?GetThisClass@CHttpConnection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHttpConnection__SAPEAUCRuntimeClass__XZ() {
    return CHttpConnection::GetThisClass();
}
CHttpConnection::CHttpConnection(CInternetSession* pSession, HINTERNET hConnected,
                                   const wchar_t* pstrServer, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, INTERNET_DEFAULT_HTTP_PORT, dwContext)
{
    m_hConnection = hConnected;
    memset(_httpconn_padding, 0, sizeof(_httpconn_padding));
}
CHttpConnection::CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                   INTERNET_PORT nPort, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext)
{
    memset(_httpconn_padding, 0, sizeof(_httpconn_padding));

    if (m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            nullptr,  // UserName
            nullptr,  // Password
            INTERNET_SERVICE_HTTP,
            0,
            dwContext
        );
    }
}
CHttpConnection::CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                   INTERNET_PORT nPort, const wchar_t* pstrUserName,
                                   const wchar_t* pstrPassword, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext)
{
    memset(_httpconn_padding, 0, sizeof(_httpconn_padding));

    if (m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            pstrUserName,
            pstrPassword,
            INTERNET_SERVICE_HTTP,
            0,
            dwContext
        );
    }
}
CHttpConnection::CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                   DWORD dwFlags, INTERNET_PORT nPort, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext)
{
    memset(_httpconn_padding, 0, sizeof(_httpconn_padding));

    if (m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            nullptr,
            nullptr,
            INTERNET_SERVICE_HTTP,
            dwFlags,
            dwContext
        );
    }
}
CHttpConnection::CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                   DWORD dwFlags, INTERNET_PORT nPort,
                                   const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                   DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext)
{
    memset(_httpconn_padding, 0, sizeof(_httpconn_padding));

    if (m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            pstrUserName,
            pstrPassword,
            INTERNET_SERVICE_HTTP,
            dwFlags,
            dwContext
        );
    }
}
CHttpConnection::~CHttpConnection() {
}
CHttpFile* CHttpConnection::OpenRequest(const wchar_t* pstrVerb,
                                         const wchar_t* pstrObjectName,
                                         const wchar_t* pstrReferer,
                                         DWORD_PTR dwContext,
                                         const wchar_t** pstrAcceptTypes,
                                         const wchar_t* pstrVersion,
                                         DWORD dwFlags) {
    if (!m_hConnection) return nullptr;

    HINTERNET hRequest = HttpOpenRequestW(
        m_hConnection,
        pstrVerb ? pstrVerb : L"GET",
        pstrObjectName,
        pstrVersion,
        pstrReferer,
        pstrAcceptTypes,
        dwFlags,
        dwContext
    );

    if (!hRequest) return nullptr;

    return new CHttpFile(hRequest, m_hConnection, pstrVerb, pstrObjectName, this);
}
CHttpFile* CHttpConnection::OpenRequest(DWORD dwRequestFlags,
                                         const wchar_t* pstrVerb,
                                         const wchar_t* pstrObjectName,
                                         const wchar_t* pstrReferer,
                                         DWORD_PTR dwContext,
                                         const wchar_t** pstrAcceptTypes,
                                         const wchar_t* pstrVersion,
                                         DWORD dwFlags) {
    if (!m_hConnection) return nullptr;

    HINTERNET hRequest = HttpOpenRequestW(
        m_hConnection,
        pstrVerb ? pstrVerb : L"GET",
        pstrObjectName,
        pstrVersion,
        pstrReferer,
        pstrAcceptTypes,
        dwFlags | dwRequestFlags,
        dwContext
    );

    if (!hRequest) return nullptr;

    return new CHttpFile(hRequest, m_hConnection, pstrVerb, pstrObjectName, this);
}
