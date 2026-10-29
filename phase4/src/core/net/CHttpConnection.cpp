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
// Retained pre-existing C++ approximation: retail's integer export treats its first
// argument as a verb selector (POST, GET, HEAD, PUT, LINK, DELETE, UNLINK), and
// substitutes the connection context when dwContext is 1. This helper instead ORs
// the integer into WinInet flags and passes the context through; the export thunk below
// follows the retail behavior without changing this pre-existing implementation.
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

// === Moved from ManualThunks.cpp ===
// Symbol: ?OpenRequest@CHttpConnection@@QEAAPEAVCHttpFile@@HPEB_W0_KPEAPEB_W0K@Z
// Retail mfc140u entry RVA 0x230700 maps verb indices 0..6 to POST, GET, HEAD,
// PUT, LINK, DELETE, UNLINK and delegates to the string overload at entry RVA
// 0x230650. That body substitutes the connection context for dwContext == 1,
// defaults the HTTP version to HTTP/1.0, calls HttpOpenRequestW, then constructs
// CHttpFile. Retail reads its context at this+0x10; use OpenMFC's typed member here
// because CInternetConnection has a different member order in this implementation.
extern "C" CHttpFile* MS_ABI impl__OpenRequest_CHttpConnection__QEAAPEAVCHttpFile__HPEB_W0_KPEAPEB_W0K_Z(
    CHttpConnection* pThis, int nVerb, const wchar_t* pstrObjectName,
    const wchar_t* pstrReferer, unsigned __int64 dwContext,
    const wchar_t** pstrAcceptTypes, const wchar_t* pstrVersion, unsigned long dwFlags) {
    static const wchar_t* const verbs[] = {
        L"POST", L"GET", L"HEAD", L"PUT", L"LINK", L"DELETE", L"UNLINK"
    };
    const wchar_t* pstrVerb = (nVerb >= 0 && nVerb < 7) ? verbs[nVerb] : L"";
    const DWORD_PTR context = dwContext == 1 ? pThis->m_dwContext : dwContext;
    const wchar_t* version = pstrVersion ? pstrVersion : L"HTTP/1.0";

    HINTERNET hRequest = HttpOpenRequestW(
        pThis->m_hConnection, pstrVerb, pstrObjectName, version, pstrReferer,
        pstrAcceptTypes, dwFlags, context);
    if (!hRequest) return nullptr;

    return new CHttpFile(hRequest, pThis->m_hConnection, pstrVerb, pstrObjectName, pThis);
}

