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

//=============================================================================
// Additional CInternetSession methods
//=============================================================================
CStdioFile* CInternetSession::OpenURL(const wchar_t* pstrURL, DWORD_PTR dwContext,
                                       DWORD dwFlags, const wchar_t* pstrHeaders,
                                       DWORD dwHeadersLength) {
    (void)dwContext; (void)dwFlags; (void)pstrHeaders; (void)dwHeadersLength;
    HINTERNET hUrl = ::InternetOpenUrlW(m_hSession, pstrURL, nullptr, 0, 
                                         INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 
                                         (DWORD_PTR)dwContext);
    if (!hUrl) return nullptr;
    CHttpFile* pFile = new CHttpFile(hUrl, nullptr, L"GET", pstrURL, nullptr);
    return pFile;
}

DWORD CInternetSession::GetServiceType() const {
    return m_dwAccessType;
}

int CInternetSession::GetServiceTypeFromHandle(HINTERNET hConnect) {
    (void)hConnect;
    return INTERNET_SERVICE_HTTP;
}

int CInternetSession::GetFtpConnection(const wchar_t* pstrServer, 
                                        const wchar_t* pstrUserName,
                                        const wchar_t* pstrPassword,
                                        INTERNET_PORT nPort, int bPassive,
                                        CFtpConnection*& refConnection) {
    (void)pstrServer; (void)pstrUserName; (void)pstrPassword; (void)nPort; (void)bPassive;
    refConnection = nullptr;
    return 0;
}

int CInternetSession::GetHttpConnection(const wchar_t* pstrServer, INTERNET_PORT nPort,
                                         const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                         CHttpConnection*& refConnection) {
    (void)pstrServer; (void)nPort; (void)pstrUserName; (void)pstrPassword;
    refConnection = nullptr;
    return 0;
}

int CInternetSession::GetHttpConnection(const wchar_t* pstrServer, DWORD dwFlags,
                                         INTERNET_PORT nPort,
                                         const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                         CHttpConnection*& refConnection) {
    (void)pstrServer; (void)dwFlags; (void)nPort; (void)pstrUserName; (void)pstrPassword;
    refConnection = nullptr;
    return 0;
}

CGopherConnection* CInternetSession::GetGopherConnection(const wchar_t* pstrServer,
                                                          const wchar_t* pstrUserName,
                                                          const wchar_t* pstrPassword,
                                                          INTERNET_PORT nPort) {
    (void)pstrServer; (void)pstrUserName; (void)pstrPassword; (void)nPort;
    return nullptr;
}

//=============================================================================
// Additional CHttpFile methods
//=============================================================================
int CHttpFile::QueryInfo(DWORD dwInfoLevel, CString& str, DWORD* pdwIndex) {
    (void)dwInfoLevel; (void)pdwIndex;
    str.Empty();
    return 0;
}

int CHttpFile::QueryInfo(DWORD dwInfoLevel, SYSTEMTIME* pSysTime, DWORD* pdwIndex) {
    (void)dwInfoLevel; (void)pdwIndex;
    if (pSysTime) memset(pSysTime, 0, sizeof(SYSTEMTIME));
    return 0;
}

int CHttpFile::QueryInfoStatusCode(DWORD_PTR& dwStatusCode) const {
    dwStatusCode = 0;
    if (!m_hFile) return 0;
    DWORD dwCode = 0;
    DWORD dwSize = sizeof(DWORD);
    if (::HttpQueryInfoW(m_hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                          &dwCode, &dwSize, nullptr)) {
        dwStatusCode = dwCode;
        return 1;
    }
    return 0;
}

int CHttpFile::SendRequestEx(LPINTERNET_BUFFERS lpBuffIn, LPINTERNET_BUFFERS lpBuffOut,
                              DWORD dwFlags, DWORD_PTR dwContext) {
    (void)lpBuffIn; (void)lpBuffOut; (void)dwFlags; (void)dwContext;
    return 0;
}

//=============================================================================
// Additional CFtpConnection methods
//=============================================================================
int CFtpConnection::GetCurrentDirectory(wchar_t* pstrDirName, DWORD* pdwLen) const {
    if (!m_hConnection || !pstrDirName || !pdwLen) return 0;
    return ::FtpGetCurrentDirectoryW(m_hConnection, pstrDirName, pdwLen) ? 1 : 0;
}

CInternetFile* CFtpConnection::Command(const wchar_t* pstrCommand, CmdResponseType eResponse,
                                        CmdResponseType eResponse2, unsigned long dwContext,
                                        unsigned __int64 dwFlags) {
    (void)pstrCommand; (void)eResponse; (void)eResponse2; (void)dwContext; (void)dwFlags;
    return nullptr;
}

int CFtpConnection::Command(const wchar_t* pstrCommand, DWORD dwCmdResponse,
                             DWORD_PTR dwContext) {
    (void)pstrCommand; (void)dwCmdResponse; (void)dwContext;
    return 0;
}

//=============================================================================
// CGopherLocator — defined in include/openmfc/afxinet.h
//=============================================================================

//=============================================================================
// CFileFind implementations
//=============================================================================
IMPLEMENT_DYNAMIC(CFileFind, CObject)

CFileFind::CFileFind()
    : m_hFindFile(INVALID_HANDLE_VALUE), m_bGotFirst(0) {
    memset(&m_findData, 0, sizeof(m_findData));
    memset(_filefind_padding, 0, sizeof(_filefind_padding));
}

CFileFind::~CFileFind() { Close(); }

int CFileFind::FindFile(const wchar_t* pstrName, DWORD dwUnused) {
    (void)dwUnused;
    Close();
    m_hFindFile = ::FindFirstFileW(pstrName, &m_findData);
    if (m_hFindFile == INVALID_HANDLE_VALUE) return 0;
    m_bGotFirst = 1;
    m_strFileName = m_findData.cFileName;
    return 1;
}

int CFileFind::FindNextFile() {
    if (m_hFindFile == INVALID_HANDLE_VALUE) return 0;
    if (!m_bGotFirst) return 0;
    if (!::FindNextFileW(m_hFindFile, &m_findData)) return 0;
    m_strFileName = m_findData.cFileName;
    return 1;
}

void CFileFind::Close() {
    if (m_hFindFile != INVALID_HANDLE_VALUE) {
        ::FindClose(m_hFindFile);
        m_hFindFile = INVALID_HANDLE_VALUE;
    }
    m_bGotFirst = 0;
}

int CFileFind::IsDirectory() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; }
int CFileFind::IsDots() const { return m_strFileName == L"." || m_strFileName == L".."; }
int CFileFind::IsArchived() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0; }
int CFileFind::IsCompressed() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0; }
int CFileFind::IsHidden() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0; }
int CFileFind::IsNormal() const { return m_findData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL; }
int CFileFind::IsReadOnly() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0; }
int CFileFind::IsSystem() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0; }
int CFileFind::IsTemporary() const { return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0; }
int CFileFind::MatchesMask(DWORD dwMask) const { return (m_findData.dwFileAttributes & dwMask) != 0; }
ULONGLONG CFileFind::GetLength() const { return ((ULONGLONG)m_findData.nFileSizeHigh << 32) | m_findData.nFileSizeLow; }
CString CFileFind::GetFileName() const { return m_strFileName; }
CString CFileFind::GetFilePath() const { return m_strRoot + m_strFileName; }
CString CFileFind::GetFileTitle() const { 
    CString s = m_strFileName;
    int pos = s.ReverseFind(L'.');
    return (pos > 0) ? s.Left(pos) : s;
}
CString CFileFind::GetFileURL() const { return L"file://" + GetFilePath(); }
CString CFileFind::GetRoot() const { return m_strRoot; }

int CFileFind::GetLastWriteTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftLastWriteTime; return 1; }
    return 0;
}
int CFileFind::GetLastAccessTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftLastAccessTime; return 1; }
    return 0;
}
int CFileFind::GetCreationTime(FILETIME* pFileTime) const {
    if (pFileTime) { *pFileTime = m_findData.ftCreationTime; return 1; }
    return 0;
}

int CFileFind::GetFileName(CString& strFileName) {
    strFileName.Empty();
    return 0;
}

//=============================================================================
// CFtpFileFind implementations
//=============================================================================
IMPLEMENT_DYNAMIC(CFtpFileFind, CFileFind)

CFtpFileFind::CFtpFileFind(CFtpConnection* pFtpConnection, DWORD_PTR dwContext)
    : CFileFind(), m_pFtpConnection(pFtpConnection), m_dwContext(dwContext),
      m_hFindHandle(nullptr) {
    memset(&m_findFileData, 0, sizeof(m_findFileData));
    memset(_ftpfifind_padding, 0, sizeof(_ftpfifind_padding));
}

CFtpFileFind::~CFtpFileFind() { Close(); }

int CFtpFileFind::FindFile(const wchar_t* pstrName, DWORD dwFlags) {
    (void)dwFlags;
    Close();
    if (!m_pFtpConnection || !m_pFtpConnection->m_hConnection) return 0;
    m_hFindHandle = ::FtpFindFirstFileW(m_pFtpConnection->m_hConnection,
                                         pstrName, &m_findFileData, 
                                         INTERNET_FLAG_RELOAD, (DWORD_PTR)m_dwContext);
    if (!m_hFindHandle) return 0;
    m_strFileName = m_findFileData.cFileName;
    return 1;
}

int CFtpFileFind::FindNextFile() {
    if (!m_hFindHandle) return 0;
    if (!::InternetFindNextFileW(m_hFindHandle, &m_findFileData)) return 0;
    m_strFileName = m_findFileData.cFileName;
    return 1;
}

void CFtpFileFind::Close() {
    if (m_hFindHandle) {
        ::InternetCloseHandle(m_hFindHandle);
        m_hFindHandle = nullptr;
    }
    CFileFind::Close();
}

CString CFtpFileFind::GetFileURL() const {
    if (m_pFtpConnection)
        return L"ftp://" + m_pFtpConnection->GetServerName() + L"/" + m_strFileName;
    return m_strFileName;
}

//=============================================================================
// CGopherFileFind (stub)
//=============================================================================
IMPLEMENT_DYNAMIC(GopherFileFind, CFtpFileFind)

GopherFileFind::GopherFileFind(CFtpConnection* pFtp, DWORD_PTR dwContext)
    : CFtpFileFind(pFtp, dwContext) {}

GopherFileFind::~GopherFileFind() {}

CString GopherFileFind::GetFileName() const { return CString(); }
CString GopherFileFind::GetScreenName() const { return CString(); }
CGopherLocator GopherFileFind::GetLocator() const { CGopherLocator loc; return loc; }
int GopherFileFind::GetFileLength(DWORD_PTR& dwLength) const { dwLength = 0; return 0; }

//=============================================================================
// CGopherConnection (stub)
//=============================================================================
IMPLEMENT_DYNAMIC(CGopherConnection, CInternetConnection)

CGopherConnection::CGopherConnection(CInternetSession* pSession, HINTERNET hConnected,
                                      const wchar_t* pstrServer, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, INTERNET_DEFAULT_GOPHER_PORT, dwContext) {
    m_hConnection = hConnected;
    memset(_gopherconn_padding, 0, sizeof(_gopherconn_padding));
}

CGopherConnection::CGopherConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                      const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                      DWORD_PTR dwContext, INTERNET_PORT nPort)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext) {
    memset(_gopherconn_padding, 0, sizeof(_gopherconn_padding));
    (void)pstrUserName; (void)pstrPassword;
}

CGopherConnection::~CGopherConnection() {}

CGopherFile* CGopherConnection::OpenFile(GOPHER_FIND_DATAW* pFindData,
                                          const wchar_t* pstrLocator,
                                          DWORD_PTR dwContext) {
    (void)pFindData; (void)pstrLocator; (void)dwContext;
    return nullptr;
}

CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 CGopherLocator* pLocator,
                                                 unsigned long dwContext) {
    (void)pstrDisplayString; (void)pLocator; (void)dwContext;
    CGopherLocator loc; return loc;
}

CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrLocator) {
    (void)pstrLocator;
    CGopherLocator loc; return loc;
}

CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 CGopherLocator* pLocator1,
                                                 CGopherLocator* pLocator2,
                                                 unsigned long dwContext,
                                                 unsigned short nGopherType) {
    (void)pstrDisplayString; (void)pLocator1; (void)pLocator2; (void)dwContext; (void)nGopherType;
    CGopherLocator loc; return loc;
}

// Convenience overload
CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 const wchar_t* pstrSelectorString,
                                                 DWORD dwGopherType) {
    CGopherLocator loc;
    (void)pstrDisplayString; (void)pstrSelectorString; (void)dwGopherType;
    return loc;
}

CString CGopherConnection::GetLocatorType(const CGopherLocator& refLocator) {
    (void)refLocator;
    return CString();
}

CFtpFileFind* CGopherConnection::CreateFileFind(CGopherLocator* pLocator) {
    (void)pLocator;
    return nullptr;
}

//=============================================================================
// CGopherFile (stub)
//=============================================================================
CGopherFile::CGopherFile(HINTERNET hFile, CGopherLocator& refLocator,
                         CGopherConnection* pConnection)
    : CInternetFile() {
    m_hFile = hFile;
    (void)refLocator; (void)pConnection;
}

CGopherFile::~CGopherFile() {}
