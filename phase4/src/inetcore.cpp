// Internet (WinInet) Implementation
// Provides MFC wrappers around WinInet API for HTTP, FTP, Gopher
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxstr.h"
#include <windows.h>
#include <wininet.h>
#include <cstring>
#include <cstdlib>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// CInternetSession
//=============================================================================

IMPLEMENT_DYNAMIC(CInternetSession, CObject)

// Map for looking up sessions by handle
#include <unordered_map>
static std::unordered_map<HINTERNET, CInternetSession*> g_sessionMap;

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
    m_bCallbackEnabled = bEnable;
    if (m_hSession) {
        if (bEnable) {
            InternetSetStatusCallbackW(m_hSession, nullptr);
        } else {
            InternetSetStatusCallbackW(m_hSession, nullptr);
        }
    }
    return TRUE;
}

//=============================================================================
// CInternetConnection
//=============================================================================

IMPLEMENT_DYNAMIC(CInternetConnection, CObject)

CInternetConnection::CInternetConnection(CInternetSession* pSession,
                                           const wchar_t* pstrServer,
                                           INTERNET_PORT nPort,
                                           DWORD_PTR dwContext)
    : m_hConnection(nullptr), m_pSession(pSession), m_strServerName(pstrServer),
      m_nPort(nPort), m_dwContext(dwContext)
{
    memset(_inetconn_padding, 0, sizeof(_inetconn_padding));
}

CInternetConnection::~CInternetConnection() {
    if (m_hConnection) {
        InternetCloseHandle(m_hConnection);
        m_hConnection = nullptr;
    }
}

//=============================================================================
// CHttpConnection
//=============================================================================

IMPLEMENT_DYNAMIC(CHttpConnection, CInternetConnection)

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

//=============================================================================
// RTTI for CInternetFile and CInternetException (manual stubs)
// CStdioFile/CFile don't have DECLARE_DYNAMIC in our headers

__attribute__((used)) static CRuntimeClass g_classCInternetFile = {
    "CStdioFile", sizeof(CInternetFile), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};

__attribute__((used)) static CRuntimeClass g_classCInternetException = {
    "CInternetException", sizeof(CInternetException), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};

//=============================================================================
// CInternetFile
//=============================================================================

// Not using IMPLEMENT_DYNAMIC since base class CStdioFile lacks DECLARE_DYNAMIC
CInternetFile::CInternetFile()
    : CStdioFile(), m_hFile(nullptr), m_dwContext(0)
{
    memset(_inetfile_padding, 0, sizeof(_inetfile_padding));
}

CInternetFile::~CInternetFile() {
    if (m_hFile) {
        InternetCloseHandle(m_hFile);
        m_hFile = nullptr;
    }
}

ULONGLONG CInternetFile::GetLength() const {
    if (!m_hFile) return 0;
    DWORD dwSize = 0;
    DWORD dwSizeLen = sizeof(dwSize);
    if (HttpQueryInfoW(m_hFile, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
                        &dwSize, &dwSizeLen, nullptr)) {
        return (ULONGLONG)dwSize;
    }
    return 0;
}

ULONGLONG CInternetFile::Seek(LONGLONG lOff, UINT nFrom) {
    (void)lOff; (void)nFrom;
    return (ULONGLONG)-1;  // Internet files don't support seek
}

UINT CInternetFile::Read(void* lpBuf, UINT nCount) {
    if (!m_hFile) return 0;
    DWORD dwRead = 0;
    if (InternetReadFile(m_hFile, lpBuf, nCount, &dwRead)) {
        return dwRead;
    }
    return 0;
}

int CInternetFile::SetReadBufferSize(UINT nReadSize) {
    if (!m_hFile) return FALSE;
    return InternetSetOptionW(m_hFile, INTERNET_OPTION_READ_BUFFER_SIZE,
                               &nReadSize, sizeof(nReadSize));
}

int CInternetFile::SetWriteBufferSize(UINT nWriteSize) {
    if (!m_hFile) return FALSE;
    return InternetSetOptionW(m_hFile, INTERNET_OPTION_WRITE_BUFFER_SIZE,
                               &nWriteSize, sizeof(nWriteSize));
}

//=============================================================================
// CHttpFile
//=============================================================================

// CHttpFile RTTI provided manually (CInternetFile doesn't have DECLARE_DYNAMIC)
CHttpFile::CHttpFile(HINTERNET hFile, HINTERNET hConnect,
                     const wchar_t* pstrVerb, const wchar_t* pstrObjectName,
                     CHttpConnection* pConnection)
    : CInternetFile(), m_bHttps(FALSE), m_pConnection(pConnection),
      m_strVerb(pstrVerb ? pstrVerb : L""),
      m_strObject(pstrObjectName ? pstrObjectName : L""),
      m_hConnect(hConnect)
{
    m_hFile = hFile;
    memset(_httpfile_padding, 0, sizeof(_httpfile_padding));
}

CHttpFile::~CHttpFile() {
}

int CHttpFile::AddRequestHeaders(const wchar_t* pstrHeaders, DWORD dwFlags) {
    if (!m_hFile) return FALSE;
    return HttpAddRequestHeadersW(m_hFile, pstrHeaders, (DWORD)-1, dwFlags);
}

int CHttpFile::AddRequestHeaders(CString& str, DWORD dwFlags) {
    return AddRequestHeaders((const wchar_t*)str, dwFlags);
}

int CHttpFile::SendRequest(const wchar_t* pstrHeaders, DWORD dwHeadersLen,
                            void* lpOptional, DWORD dwOptionalLen) {
    if (!m_hFile) return FALSE;
    return HttpSendRequestW(m_hFile, pstrHeaders, dwHeadersLen, lpOptional, dwOptionalLen);
}

int CHttpFile::SendRequest(CString& strHeaders, DWORD dwHeadersLen,
                            void* lpOptional, DWORD dwOptionalLen) {
    return SendRequest((const wchar_t*)strHeaders, dwHeadersLen, lpOptional, dwOptionalLen);
}

int CHttpFile::SendRequestEx(DWORD dwTotalLen, DWORD dwFlags, DWORD_PTR dwContext) {
    if (!m_hFile) return FALSE;
    INTERNET_BUFFERS buffer = {};
    buffer.dwStructSize = sizeof(buffer);
    buffer.dwBufferTotal = dwTotalLen;
    return HttpSendRequestExW(m_hFile, &buffer, nullptr, dwFlags, dwContext);
}

int CHttpFile::EndRequest(DWORD dwFlags, LPINTERNET_BUFFERS lpBuffIn, DWORD_PTR dwContext) {
    if (!m_hFile) return FALSE;
    return HttpEndRequestW(m_hFile, lpBuffIn, dwFlags, dwContext);
}

int CHttpFile::QueryInfo(DWORD dwInfoLevel, void* lpBuffer, DWORD* pdwBufferLength,
                          DWORD* pdwIndex) {
    if (!m_hFile) return FALSE;
    return HttpQueryInfoW(m_hFile, dwInfoLevel, lpBuffer, pdwBufferLength, pdwIndex);
}

int CHttpFile::QueryInfoStatusCode(DWORD& dwStatusCode) const {
    if (!m_hFile) return FALSE;
    DWORD dwSize = sizeof(dwStatusCode);
    return HttpQueryInfoW(m_hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                           &dwStatusCode, &dwSize, nullptr);
}

CString CHttpFile::GetVerb() const {
    return m_strVerb;
}

CString CHttpFile::GetObject() const {
    return m_strObject;
}

CString CHttpFile::GetFileURL() const {
    CString url = L"http";
    if (m_bHttps) url += L"s";
    url += L"://";
    if (m_pConnection) {
        url += m_pConnection->GetServerName();
    }
    url += m_strObject;
    return url;
}

void CHttpFile::Close() {
    if (m_hFile) {
        InternetCloseHandle(m_hFile);
    }
}

//=============================================================================
// CFtpConnection
//=============================================================================

IMPLEMENT_DYNAMIC(CFtpConnection, CInternetConnection)

CFtpConnection::CFtpConnection(CInternetSession* pSession, HINTERNET hConnected,
                                 const wchar_t* pstrServer, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, INTERNET_DEFAULT_FTP_PORT, dwContext)
{
    m_hConnection = hConnected;
    memset(_ftpconn_padding, 0, sizeof(_ftpconn_padding));
}

CFtpConnection::CFtpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                 const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                 DWORD_PTR dwContext, INTERNET_PORT nPort, int bPassive)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext)
{
    memset(_ftpconn_padding, 0, sizeof(_ftpconn_padding));

    if (m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            pstrUserName,
            pstrPassword,
            INTERNET_SERVICE_FTP,
            bPassive ? INTERNET_FLAG_PASSIVE : 0,
            dwContext
        );
    }
}

CFtpConnection::~CFtpConnection() {
}

int CFtpConnection::GetCurrentDirectory(CString& strDirName) const {
    if (!m_hConnection) return FALSE;
    wchar_t buf[MAX_PATH];
    DWORD len = MAX_PATH;
    if (FtpGetCurrentDirectoryW(m_hConnection, buf, &len)) {
        strDirName = buf;
        return TRUE;
    }
    return FALSE;
}

int CFtpConnection::SetCurrentDirectory(const wchar_t* pstrDirName) {
    if (!m_hConnection) return FALSE;
    return FtpSetCurrentDirectoryW(m_hConnection, pstrDirName);
}

int CFtpConnection::CreateDirectory(const wchar_t* pstrDirName) {
    if (!m_hConnection) return FALSE;
    return FtpCreateDirectoryW(m_hConnection, pstrDirName);
}

int CFtpConnection::RemoveDirectory(const wchar_t* pstrDirName) {
    if (!m_hConnection) return FALSE;
    return FtpRemoveDirectoryW(m_hConnection, pstrDirName);
}

int CFtpConnection::GetFile(const wchar_t* pstrRemoteFile, const wchar_t* pstrLocalFile,
                              int bFailIfExists, DWORD dwFlagsAndAttributes,
                              DWORD dwFlags, DWORD_PTR dwContext) {
    (void)bFailIfExists; (void)dwContext;
    if (!m_hConnection) return FALSE;
    return FtpGetFileW(m_hConnection, pstrRemoteFile, pstrLocalFile,
                        bFailIfExists, dwFlagsAndAttributes, dwFlags, 0);
}

int CFtpConnection::PutFile(const wchar_t* pstrLocalFile, const wchar_t* pstrRemoteFile,
                              DWORD dwFlags, DWORD_PTR dwContext) {
    (void)dwContext;
    if (!m_hConnection) return FALSE;
    return FtpPutFileW(m_hConnection, pstrLocalFile, pstrRemoteFile, dwFlags, 0);
}

int CFtpConnection::Rename(const wchar_t* pstrExisting, const wchar_t* pstrNew) {
    if (!m_hConnection) return FALSE;
    return FtpRenameFileW(m_hConnection, pstrExisting, pstrNew);
}

int CFtpConnection::Remove(const wchar_t* pstrFileName) {
    if (!m_hConnection) return FALSE;
    return FtpDeleteFileW(m_hConnection, pstrFileName);
}

CInternetFile* CFtpConnection::OpenFile(const wchar_t* pstrFileName,
                                          DWORD dwAccess, DWORD dwFlags,
                                          DWORD_PTR dwContext) {
    if (!m_hConnection) return nullptr;
    HINTERNET hFile = FtpOpenFileW(m_hConnection, pstrFileName, dwAccess, dwFlags, dwContext);
    if (!hFile) return nullptr;

    CInternetFile* pFile = new CInternetFile();
    pFile->m_hFile = hFile;
    return pFile;
}

void CFtpConnection::Close() {
    if (m_hConnection) {
        InternetCloseHandle(m_hConnection);
        m_hConnection = nullptr;
    }
}

//=============================================================================
// CInternetException
//=============================================================================

IMPLEMENT_DYNAMIC(CInternetException, CException)

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
