// OpenMFC Internet (WinInet) Classes
// ABI-compatible declarations for mfc140u.dll internet/socket exports
#pragma once

#ifndef OPENMFC_AFXINET_H
#define OPENMFC_AFXINET_H

#include "afx.h"
#include <wininet.h>

//=============================================================================
// Forward declarations
//=============================================================================
class CInternetSession;
class CInternetConnection;
class CInternetFile;
class CHttpConnection;
class CHttpFile;
class CFtpConnection;
class CFtpFileFind;
class CGopherConnection;
class CGopherFile;
class CGopherFileFind;
class CGopherLocator;
class CFileFind;

//=============================================================================
// CInternetSession - Manages WinInet sessions
//=============================================================================
class CInternetSession : public CObject {
    DECLARE_DYNAMIC(CInternetSession)
public:
    CInternetSession(const wchar_t* pstrAgent = nullptr,
                     DWORD_PTR dwContext = 1,
                     DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG,
                     const wchar_t* pstrProxyName = nullptr,
                     const wchar_t* pstrProxyBypass = nullptr,
                     DWORD dwFlags = 0);
    virtual ~CInternetSession();

    // Operations
    CHttpConnection* GetHttpConnection(const wchar_t* pstrServer,
                                        INTERNET_PORT nPort = INTERNET_DEFAULT_HTTP_PORT,
                                        const wchar_t* pstrUserName = nullptr,
                                        const wchar_t* pstrPassword = nullptr);
    CHttpConnection* GetHttpConnection(const wchar_t* pstrServer,
                                        DWORD dwFlags,
                                        INTERNET_PORT nPort = INTERNET_DEFAULT_HTTP_PORT,
                                        const wchar_t* pstrUserName = nullptr,
                                        const wchar_t* pstrPassword = nullptr);
    CFtpConnection* GetFtpConnection(const wchar_t* pstrServer,
                                      const wchar_t* pstrUserName = nullptr,
                                      const wchar_t* pstrPassword = nullptr,
                                      INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT,
                                      int bPassive = FALSE);
    CGopherConnection* GetGopherConnection(const wchar_t* pstrServer,
                                            const wchar_t* pstrUserName = nullptr,
                                            const wchar_t* pstrPassword = nullptr,
                                            INTERNET_PORT nPort = INTERNET_DEFAULT_GOPHER_PORT);
    CStdioFile* OpenURL(const wchar_t* pstrURL,
                         DWORD_PTR dwContext = 1,
                         DWORD dwFlags = INTERNET_FLAG_TRANSFER_ASCII,
                         const wchar_t* pstrHeaders = nullptr,
                         DWORD dwHeadersLength = 0);
    void Close();
    int EnableStatusCallback(int bEnable = TRUE);

    // Query
    HINTERNET GetHandle() const { return m_hSession; }
    int QueryOption(DWORD dwOption, void* lpBuffer, DWORD* pdwBufLen) const;
    int SetOption(DWORD dwOption, void* lpBuffer, DWORD dwBufLen);

    // Static helpers
    static int GetFtpConnection(const wchar_t*, const wchar_t*, const wchar_t*,
                                 INTERNET_PORT, int, CFtpConnection*&);
    static int GetHttpConnection(const wchar_t*, INTERNET_PORT,
                                  const wchar_t*, const wchar_t*, CHttpConnection*&);
    static int GetHttpConnection(const wchar_t*, DWORD, INTERNET_PORT,
                                  const wchar_t*, const wchar_t*, CHttpConnection*&);

    // Service type
    DWORD GetServiceType() const;
    static int GetServiceTypeFromHandle(HINTERNET hConnect);

public:
    HINTERNET m_hSession;
    DWORD_PTR m_dwContext;
    DWORD m_dwAccessType;
    CString m_strAgent;
    int m_bCallbackEnabled;

protected:
    char _inetsession_padding[32];
};

//=============================================================================
// CInternetConnection - Base for protocol connections
//=============================================================================
class CInternetConnection : public CObject {
    DECLARE_DYNAMIC(CInternetConnection)
public:
    CInternetConnection(CInternetSession* pSession,
                        const wchar_t* pstrServer,
                        INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
                        DWORD_PTR dwContext = 1);
    virtual ~CInternetConnection();

    HINTERNET GetHandle() const { return m_hConnection; }
    CInternetSession* GetSession() const { return m_pSession; }
    const CString& GetServerName() const { return m_strServerName; }

public:
    HINTERNET m_hConnection;
    CInternetSession* m_pSession;
    CString m_strServerName;
    INTERNET_PORT m_nPort;
    DWORD_PTR m_dwContext;

protected:
    char _inetconn_padding[24];
};

//=============================================================================
// CHttpConnection - HTTP protocol connection
//=============================================================================
class CHttpConnection : public CInternetConnection {
    DECLARE_DYNAMIC(CHttpConnection)
public:
    CHttpConnection(CInternetSession* pSession, HINTERNET hConnected,
                    const wchar_t* pstrServer, DWORD_PTR dwContext);
    CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                    INTERNET_PORT nPort = INTERNET_DEFAULT_HTTP_PORT,
                    DWORD_PTR dwContext = 1);
    CHttpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                    DWORD dwFlags, INTERNET_PORT nPort = INTERNET_DEFAULT_HTTP_PORT,
                    DWORD_PTR dwContext = 1);
    virtual ~CHttpConnection();

    CHttpFile* OpenRequest(const wchar_t* pstrVerb,
                            const wchar_t* pstrObjectName,
                            const wchar_t* pstrReferer = nullptr,
                            DWORD_PTR dwContext = 1,
                            const wchar_t** pstrAcceptTypes = nullptr,
                            const wchar_t* pstrVersion = nullptr,
                            DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT);
    CHttpFile* OpenRequest(DWORD dwRequestFlags,
                            const wchar_t* pstrVerb,
                            const wchar_t* pstrObjectName,
                            const wchar_t* pstrReferer = nullptr,
                            DWORD_PTR dwContext = 1,
                            const wchar_t** pstrAcceptTypes = nullptr,
                            const wchar_t* pstrVersion = nullptr,
                            DWORD dwFlags = INTERNET_FLAG_EXISTING_CONNECT);

public:
    CString m_strServerName;  // duplicate? inherited has one too

protected:
    char _httpconn_padding[40];
};

//=============================================================================
// CInternetFile - Base for internet-based files
//=============================================================================
class CInternetFile : public CStdioFile {
    // Note: CStdioFile/CFile don't extend CObject in OpenMFC headers
    // RTTI symbols provided manually in inetcore.cpp
public:
    CInternetFile();
    virtual ~CInternetFile();

    HINTERNET GetHandle() const { return m_hFile; }
    virtual ULONGLONG GetLength() const override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;

    // Read/Write override
    virtual UINT Read(void* lpBuf, UINT nCount) override;

    // Internet-specific
    int SetReadBufferSize(UINT nReadSize);
    int SetWriteBufferSize(UINT nWriteSize);

public:
    HINTERNET m_hFile;
    DWORD m_dwContext;

protected:
    char _inetfile_padding[40];
};

//=============================================================================
// CHttpFile - HTTP file (response from HTTP request)
//=============================================================================
class CHttpFile : public CInternetFile {
    // RTTI handled via extern "C" exports
public:
    CHttpFile(HINTERNET hFile, HINTERNET hConnect,
              const wchar_t* pstrVerb, const wchar_t* pstrObjectName,
              CHttpConnection* pConnection);
    virtual ~CHttpFile();

    // HTTP-specific operations
    int AddRequestHeaders(const wchar_t* pstrHeaders, DWORD dwFlags = HTTP_ADDREQ_FLAG_ADD_IF_NEW);
    int AddRequestHeaders(CString& str, DWORD dwFlags = HTTP_ADDREQ_FLAG_ADD_IF_NEW);
    int SendRequest(const wchar_t* pstrHeaders = nullptr, DWORD dwHeadersLen = 0,
                    void* lpOptional = nullptr, DWORD dwOptionalLen = 0);
    int SendRequest(CString& strHeaders, DWORD dwHeadersLen = 0,
                    void* lpOptional = nullptr, DWORD dwOptionalLen = 0);
    int SendRequestEx(DWORD dwTotalLen,
                       DWORD dwFlags = HSR_INITIATE,
                       DWORD_PTR dwContext = 1);
    int SendRequestEx(LPINTERNET_BUFFERS lpBuffIn, LPINTERNET_BUFFERS lpBuffOut,
                       DWORD dwFlags = HSR_INITIATE, DWORD_PTR dwContext = 1);
    int EndRequest(DWORD dwFlags = 0,
                    LPINTERNET_BUFFERS lpBuffIn = nullptr,
                    DWORD_PTR dwContext = 1);
    int QueryInfo(DWORD dwInfoLevel, void* lpBuffer, DWORD* pdwBufferLength,
                   DWORD* pdwIndex = nullptr);
    int QueryInfo(DWORD dwInfoLevel, CString& str,
                   DWORD* pdwIndex = nullptr);
    int QueryInfo(DWORD dwInfoLevel, SYSTEMTIME* pSysTime,
                   DWORD* pdwIndex = nullptr);
    int QueryInfoStatusCode(DWORD& dwStatusCode) const;
    int QueryInfoStatusCode(DWORD_PTR& dwStatusCode) const;
    CString GetVerb() const;
    CString GetObject() const;
    CString GetFileURL() const;
    void Close();

public:
    int m_bHttps;
    CHttpConnection* m_pConnection;
    CString m_strVerb;
    CString m_strObject;
    HINTERNET m_hConnect;  // duplicate?

protected:
    char _httpfile_padding[40];
};

//=============================================================================
// CFtpConnection - FTP protocol connection
//=============================================================================
class CFtpConnection : public CInternetConnection {
    DECLARE_DYNAMIC(CFtpConnection)
public:
    // Internal command response type enum (for internal use)
    enum CmdResponseType {
        CmdRespNone = 0,
        CmdRespRead = 1,
        CmdRespWrite = 2
    };

    CFtpConnection(CInternetSession* pSession, HINTERNET hConnected,
                    const wchar_t* pstrServer, DWORD_PTR dwContext);
    CFtpConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                    const wchar_t* pstrUserName = nullptr,
                    const wchar_t* pstrPassword = nullptr,
                    DWORD_PTR dwContext = 1,
                    INTERNET_PORT nPort = INTERNET_DEFAULT_FTP_PORT,
                    int bPassive = FALSE);
    virtual ~CFtpConnection();

    // Directory operations
    int GetCurrentDirectory(CString& strDirName) const;
    int GetCurrentDirectory(wchar_t* pstrDirName, DWORD* pdwLen) const;
    int SetCurrentDirectory(const wchar_t* pstrDirName);
    int CreateDirectory(const wchar_t* pstrDirName);
    int RemoveDirectory(const wchar_t* pstrDirName);

    // File operations
    int GetFile(const wchar_t* pstrRemoteFile, const wchar_t* pstrLocalFile,
                int bFailIfExists = TRUE,
                DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
                DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
                DWORD_PTR dwContext = 1);
    int PutFile(const wchar_t* pstrLocalFile, const wchar_t* pstrRemoteFile,
                DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
                DWORD_PTR dwContext = 1);
    int Rename(const wchar_t* pstrExisting, const wchar_t* pstrNew);
    int Remove(const wchar_t* pstrFileName);
    CInternetFile* OpenFile(const wchar_t* pstrFileName,
                             DWORD dwAccess = GENERIC_READ,
                             DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
                             DWORD_PTR dwContext = 1);

    // Command
    CInternetFile* Command(const wchar_t* pstrCommand,
                            CmdResponseType eResponse = CmdRespNone,
                            CmdResponseType eResponse2 = CmdRespNone,
                            unsigned long dwContext = 0,
                            unsigned __int64 dwFlags = 0);
    int Command(const wchar_t* pstrCommand,
                 DWORD dwCmdResponse = FTP_TRANSFER_TYPE_ASCII,
                 DWORD_PTR dwContext = 1);

    void Close();

protected:
    char _ftpconn_padding[32];
};

typedef CFtpConnection::CmdResponseType CmdResponseType;

//=============================================================================
// CGopherConnection - Gopher protocol connection
//=============================================================================
class CGopherConnection : public CInternetConnection {
    DECLARE_DYNAMIC(CGopherConnection)
public:
    CGopherConnection(CInternetSession* pSession, HINTERNET hConnected,
                       const wchar_t* pstrServer, DWORD_PTR dwContext);
    CGopherConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                       const wchar_t* pstrUserName = nullptr,
                       const wchar_t* pstrPassword = nullptr,
                       DWORD_PTR dwContext = 1,
                       INTERNET_PORT nPort = INTERNET_DEFAULT_GOPHER_PORT);
    virtual ~CGopherConnection();

    CGopherFile* OpenFile(GOPHER_FIND_DATAW* pFindData,
                           const wchar_t* pstrLocator = nullptr,
                           DWORD_PTR dwContext = 1);
    // MSVC-compatible CreateLocator overloads
    CGopherLocator CreateLocator(const wchar_t* pstrDisplayString,
                                  CGopherLocator* pLocator,
                                  unsigned long dwContext);
    static CGopherLocator CreateLocator(const wchar_t* pstrLocator);
    static CGopherLocator CreateLocator(const wchar_t* pstrDisplayString,
                                         CGopherLocator* pLocator1,
                                         CGopherLocator* pLocator2,
                                         unsigned long dwContext,
                                         unsigned short nGopherType);
    // Convenience overload
    CGopherLocator CreateLocator(const wchar_t* pstrDisplayString,
                                  const wchar_t* pstrSelectorString,
                                  DWORD dwGopherType);
    static CString GetLocatorType(const CGopherLocator& refLocator);
    CFtpFileFind* CreateFileFind(CGopherLocator* pLocator = nullptr);

protected:
    char _gopherconn_padding[32];
};

//=============================================================================
// CGopherLocator - minimal definition (needed for retval thunks)
//=============================================================================
class CGopherLocator {
public:
    CGopherLocator() : m_dwBufferLength(0), m_lpBuffer(nullptr) {}
    ~CGopherLocator() { if (m_lpBuffer) free(m_lpBuffer); }
    DWORD m_dwBufferLength;
    void* m_lpBuffer;
    char _gopherlocator_padding[16];
};

//=============================================================================
// CFileFind - Base file finder
//=============================================================================
class CFileFind : public CObject {
    DECLARE_DYNAMIC(CFileFind)
public:
    CFileFind();
    virtual ~CFileFind();

    virtual int FindFile(const wchar_t* pstrName, DWORD dwUnused = 0);
    virtual int FindNextFile();
    virtual void Close();
    virtual int IsDirectory() const;
    virtual int IsDots() const;
    virtual int IsArchived() const;
    virtual int IsCompressed() const;
    virtual int IsHidden() const;
    virtual int IsNormal() const;
    virtual int IsReadOnly() const;
    virtual int IsSystem() const;
    virtual int IsTemporary() const;
    virtual int MatchesMask(DWORD dwMask) const;
    virtual ULONGLONG GetLength() const;
    virtual CString GetFileName() const;
    virtual CString GetFilePath() const;
    virtual CString GetFileTitle() const;
    virtual CString GetFileURL() const;
    virtual CString GetRoot() const;
    int GetLastWriteTime(FILETIME* pFileTime) const;
    int GetLastAccessTime(FILETIME* pFileTime) const;
    int GetCreationTime(FILETIME* pFileTime) const;

    static int GetFileName(CString& strFileName);

public:
    HANDLE m_hFindFile;
    WIN32_FIND_DATAW m_findData;
    CString m_strFileName;
    CString m_strRoot;
    int m_bGotFirst;

protected:
    char _filefind_padding[32];
};

//=============================================================================
// CFtpFileFind - FTP file finder
//=============================================================================
class CFtpFileFind : public CFileFind {
    DECLARE_DYNAMIC(CFtpFileFind)
public:
    CFtpFileFind(CFtpConnection* pFtpConnection, DWORD_PTR dwContext = 1);
    virtual ~CFtpFileFind();

    virtual int FindFile(const wchar_t* pstrName, DWORD dwFlags = INTERNET_FLAG_RELOAD) override;
    virtual int FindNextFile() override;
    virtual void Close() override;
    virtual CString GetFileURL() const override;

public:
    CFtpConnection* m_pFtpConnection;
    DWORD_PTR m_dwContext;
    HINTERNET m_hFindHandle;
    WIN32_FIND_DATAW m_findFileData;
    CString m_strFileName;

protected:
    char _ftpfifind_padding[32];
};

//=============================================================================
// CGopherFileFind - Gopher file finder
//=============================================================================
class GopherFileFind : public CFtpFileFind {
    DECLARE_DYNAMIC(GopherFileFind)
public:
    GopherFileFind(CFtpConnection* pFtp, DWORD_PTR dwContext = 1);
    virtual ~GopherFileFind();

    virtual CString GetFileName() const override;
    virtual CString GetScreenName() const;
    virtual CGopherLocator GetLocator() const;
    virtual int GetFileLength(DWORD_PTR& dwLength) const;
};

//=============================================================================
// CGopherFile - Gopher file
//=============================================================================
class CGopherFile : public CInternetFile {
    // RTTI handled via extern "C" exports
public:
    CGopherFile(HINTERNET hFile, CGopherLocator& refLocator,
                CGopherConnection* pConnection);
    virtual ~CGopherFile();
};

//=============================================================================
// CInternetException - Internet exception
//=============================================================================
class CInternetException : public CException {
    DECLARE_DYNAMIC(CInternetException)
public:
    CInternetException(DWORD dwError);
    virtual ~CInternetException();

    virtual int GetErrorMessage(wchar_t* lpszError, UINT nMaxError,
                                UINT* pnHelpContext = nullptr) const override;
    int GetErrorMessage(char* lpszError, UINT nMaxError,
                        UINT* pnHelpContext = nullptr) const;

public:
    DWORD m_dwError;
    DWORD m_dwContext;

protected:
    char _inetex_padding[32];
};

#endif // OPENMFC_AFXINET_H
