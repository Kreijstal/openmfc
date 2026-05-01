// OpenMFC Socket Classes
// ABI-compatible declarations for mfc140u.dll socket exports
#pragma once

#ifndef OPENMFC_AFXSOCK_H
#define OPENMFC_AFXSOCK_H

#include "afx.h"
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

    // Internal
    virtual int ConnectHelper(const struct sockaddr* lpSockAddr, int nSockAddrLen);
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

protected:
    virtual int ConnectHelper(const struct sockaddr* lpSockAddr, int nSockAddrLen) override;
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
    CSocketFile(CSocket* pSocket, int bArchiveCompatible = FALSE);
    virtual ~CSocketFile();

    virtual UINT Read(void* lpBuf, UINT nCount) override;
    virtual void Write(const void* lpBuf, UINT nCount) override;
    virtual void Close() override;
    virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom) override;

public:
    CSocket* m_pSocket;
    int m_bArchiveCompatible;

protected:
    char _socketfile_padding[32];
};

//=============================================================================
// WinSock Initialization
//=============================================================================
int AfxSocketInit(struct WSAData* lpwsaData = nullptr);

#endif // OPENMFC_AFXSOCK_H
