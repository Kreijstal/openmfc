// OpenMFC Socket Classes
// ABI-compatible declarations for mfc140u.dll socket exports
#pragma once

#ifndef OPENMFC_AFXSOCK_H
#define OPENMFC_AFXSOCK_H

#include "afx.h"
#include "afxwin.h"
#include <winsock2.h>

//=============================================================================
// CAsyncSocket - Asynchronous Windows Socket wrapper
//=============================================================================
class CAsyncSocket : public CObject {
    DECLARE_DYNAMIC(CAsyncSocket)
public:
    CAsyncSocket();
    virtual ~CAsyncSocket();

    // Creation
    int Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,
               long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
               const wchar_t* lpszSocketAddress = nullptr);

    // Attributes
    SOCKET GetSocket() const { return m_hSocket; }
    int Attach(SOCKET hSocket, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
    SOCKET Detach();
    static SOCKET DetachHandle(SOCKET hSocket, int bDeadSocket = 1);
    static void AttachHandle(SOCKET hSocket, CAsyncSocket* pSocket, int bDeadSocket = 1);

    // Operations
    int Bind(UINT nSocketPort, const wchar_t* lpszSocketAddress = nullptr);
    int Bind(const struct sockaddr* lpSockAddr, int nSockAddrLen);
    int Listen(int nConnectionBacklog = 5);
    virtual int Accept(CAsyncSocket& rConnectedSocket,
                        struct sockaddr* lpSockAddr = nullptr,
                        int* lpSockAddrLen = nullptr);
    int Connect(const wchar_t* lpszHostAddress, UINT nHostPort);
    int Connect(const struct sockaddr* lpSockAddr, int nSockAddrLen);
    virtual void Close();

    // I/O
    int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
    int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
    int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
               const wchar_t* lpszHostAddress = nullptr, int nFlags = 0);
    int SendTo(const void* lpBuf, int nBufLen, const struct sockaddr* lpSockAddr,
               int nSockAddrLen, int nFlags = 0);
    int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                    UINT& rSocketPort, int nFlags = 0);
    int ReceiveFrom(void* lpBuf, int nBufLen, struct sockaddr* lpSockAddr,
                    int* lpSockAddrLen, int nFlags = 0);

    // Events
    int AsyncSelect(long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
    int IOCtl(long lCommand, DWORD* lpArgument);

    // Information
    int GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
    int GetPeerName(struct sockaddr* lpSockAddr, int* lpSockAddrLen);
    int GetSockName(CString& rSocketAddress, UINT& rSocketPort);
    int GetSockName(struct sockaddr* lpSockAddr, int* lpSockAddrLen);
    int GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET);
    int SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel = SOL_SOCKET);
    static int GetLastError();
    static int GetLastError(int* pnErrorCode);
    static void SetLastError(int nErrorCode);

    // Raw socket creation
    int Socket(int nSocketType = SOCK_STREAM, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
               int nProtocolType = 0, int nAddressFormat = PF_INET);

    // Handle map (internal)
    static CAsyncSocket* LookupHandle(SOCKET hSocket, int bDeadSocket = 0);
    static void KillSocket(SOCKET hSocket, CAsyncSocket* pSocket);

    // Event notification callbacks (overridable)
    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    virtual void OnConnect(int nErrorCode);
    virtual void OnOutOfBandData(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);

    // Internal
    virtual int ConnectHelper(const struct sockaddr* lpSockAddr, int nSockAddrLen);
    virtual int ReceiveFromHelper(void* lpBuf, int nBufLen, struct sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags);
    virtual int SendToHelper(const void* lpBuf, int nBufLen, const struct sockaddr* lpSockAddr, int nSockAddrLen, int nFlags);
    static void PASCAL DoCallBack(SOCKET hSocket, long lParam);

protected:
    SOCKET m_hSocket;
    long m_lEvent;
    int m_nSocketType;
    int m_bConnected;

    // Padding for ABI compatibility
    char _asyncsocket_padding[40];
};

//=============================================================================
// CSocket - Blocking socket (derived from CAsyncSocket)
//=============================================================================
class CSocket : public CAsyncSocket {
    DECLARE_DYNAMIC(CSocket)
public:
    CSocket();
    virtual ~CSocket();

    // Overrides for blocking behavior
    virtual int Accept(CAsyncSocket& rConnectedSocket,
                        struct sockaddr* lpSockAddr = nullptr,
                        int* lpSockAddrLen = nullptr) override;
    virtual void Close() override;

    // Blocking operations
    int Send(const void* lpBuf, int nBufLen, int nFlags = 0);  // non-virtual, hides base
    int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
    int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
               const wchar_t* lpszHostAddress = nullptr, int nFlags = 0);
    int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                    UINT& rSocketPort, int nFlags = 0);

    // Blocking mode control
    int CancelBlockingCall();
    int IsBlocking() const;

    static void AuxQueueAdd(UINT message, SOCKET hSocket, long lParam);
    static int ProcessAuxQueue();
    int SendChunk(const void* lpBuf, int nBufLen, int nFlags);

protected:
    virtual int ConnectHelper(const struct sockaddr* lpSockAddr, int nSockAddrLen) override;
    virtual int ReceiveFromHelper(void* lpBuf, int nBufLen, struct sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags) override;
    virtual int SendToHelper(const void* lpBuf, int nBufLen, const struct sockaddr* lpSockAddr, int nSockAddrLen, int nFlags) override;
    virtual int OnMessagePending();
    int PumpMessages(UINT uStopFlag);

    int m_bBlocking;

protected:
    char _socket_padding[32];
};

//=============================================================================
// CSocketFile - CFile interface for sockets
//=============================================================================
class CSocketFile : public CFile {
public:
    // Manual RTTI (CFile doesn't derive from CObject/use DECLARE_DYNAMIC)
    static CRuntimeClass classCSocketFile;
    static CRuntimeClass* GetThisClass() { return &classCSocketFile; }
    virtual CRuntimeClass* GetRuntimeClass() const { return GetThisClass(); }

    CSocketFile(CSocket* pSocket, int bArchiveCompatible = FALSE);
    virtual ~CSocketFile();

    virtual UINT Read(void* lpBuf, UINT nCount) override;
    virtual void Write(const void* lpBuf, UINT nCount) override;
    virtual void Close() override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;
    virtual void Flush() override;
    virtual void Abort();
    virtual ULONGLONG GetLength() const override;
    virtual ULONGLONG GetPosition() const;
    virtual void SetLength(ULONGLONG dwNewLen) override;
    virtual CFile* Duplicate() const;
    virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = nullptr, void** ppBufMax = nullptr);
    virtual void LockRange(ULONGLONG dwPos, ULONGLONG dwCount);
    virtual void UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount);
    virtual int Open(const wchar_t* lpszFileName, UINT nOpenFlags, CFileException* pError = nullptr);

public:
    CSocket* m_pSocket;
    int m_bArchiveCompatible;

protected:
    char _socketfile_padding[32];
};

//=============================================================================
// CSocketWnd - Internal helper window for async socket notifications
//=============================================================================
class CSocketWnd : public CWnd {
    DECLARE_MESSAGE_MAP()
public:
    CSocketWnd();

private:
    LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
    LRESULT OnSocketDead(WPARAM wParam, LPARAM lParam);
};

//=============================================================================
// WinSock Initialization
//=============================================================================
int AfxSocketInit(struct WSAData* lpwsaData = nullptr);
void AfxSocketTerm();
int _AfxSocketInit(struct WSAData* lpwsaData);

#endif // OPENMFC_AFXSOCK_H
