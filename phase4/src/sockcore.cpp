// Socket Implementation
// Provides MFC wrappers around WinSock2 API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxsock.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//=============================================================================
// WinSock Initialization State
//=============================================================================
static int g_bSocketsInitialized = FALSE;
static WSADATA g_wsaData;

//=============================================================================
// AfxSocketInit
//=============================================================================
int AfxSocketInit(WSADATA* lpwsaData) {
    if (g_bSocketsInitialized) {
        if (lpwsaData) {
            *lpwsaData = g_wsaData;
        }
        return TRUE;
    }

    int result = WSAStartup(MAKEWORD(2, 2), &g_wsaData);
    if (result != 0) {
        return FALSE;
    }

    g_bSocketsInitialized = TRUE;

    if (lpwsaData) {
        *lpwsaData = g_wsaData;
    }
    return TRUE;
}

// Internal cleanup (reserved for DLL_PROCESS_DETACH / module termination)
__attribute__((unused)) static void AfxSocketTerm() {
    if (g_bSocketsInitialized) {
        WSACleanup();
        g_bSocketsInitialized = FALSE;
    }
}

//=============================================================================
// CAsyncSocket
//=============================================================================

IMPLEMENT_DYNAMIC(CAsyncSocket, CObject)

CAsyncSocket::CAsyncSocket()
    : m_hSocket(INVALID_SOCKET), m_lEvent(0),
      m_nSocketType(SOCK_STREAM), m_bConnected(FALSE)
{
    memset(_asyncsocket_padding, 0, sizeof(_asyncsocket_padding));
    AfxSocketInit(nullptr);
}

CAsyncSocket::~CAsyncSocket() {
    Close();
}

int CAsyncSocket::Create(UINT nSocketPort, int nSocketType,
                          long lEvent, const wchar_t* lpszSocketAddress) {
    if (m_hSocket != INVALID_SOCKET) {
        return FALSE;
    }

    m_hSocket = socket(AF_INET, nSocketType, 0);
    if (m_hSocket == INVALID_SOCKET) {
        return FALSE;
    }

    m_nSocketType = nSocketType;
    m_lEvent = lEvent;

    // Bind if port specified
    if (nSocketPort != 0 || lpszSocketAddress) {
        if (!Bind(nSocketPort, lpszSocketAddress)) {
            Close();
            return FALSE;
        }
    }

    // Enable async select
    if (lEvent != 0 && lEvent != -1) {
        AsyncSelect(lEvent);
    }

    return TRUE;
}

SOCKET CAsyncSocket::Detach() {
    SOCKET h = m_hSocket;
    if (h != INVALID_SOCKET) {
        AsyncSelect(0);
    }
    m_hSocket = INVALID_SOCKET;
    m_bConnected = FALSE;
    return h;
}

int CAsyncSocket::Attach(SOCKET hSocket, long lEvent) {
    if (m_hSocket != INVALID_SOCKET) {
        return FALSE;
    }
    if (hSocket == INVALID_SOCKET) {
        return FALSE;
    }

    m_hSocket = hSocket;
    m_lEvent = lEvent;

    if (lEvent != 0) {
        AsyncSelect(lEvent);
    }
    return TRUE;
}

int CAsyncSocket::Bind(UINT nSocketPort, const wchar_t* lpszSocketAddress) {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)nSocketPort);

    if (lpszSocketAddress && lpszSocketAddress[0]) {
        // Try to parse IP address
        InetPtonW(AF_INET, lpszSocketAddress, &addr.sin_addr);
    } else {
        addr.sin_addr.s_addr = INADDR_ANY;
    }

    return Bind((const sockaddr*)&addr, sizeof(addr));
}

int CAsyncSocket::Bind(const sockaddr* lpSockAddr, int nSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::bind(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR;
}

int CAsyncSocket::Listen(int nConnectionBacklog) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::listen(m_hSocket, nConnectionBacklog) != SOCKET_ERROR;
}

int CAsyncSocket::Accept(CAsyncSocket& rConnectedSocket,
                          sockaddr* lpSockAddr, int* lpSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    SOCKET hNew = ::accept(m_hSocket, lpSockAddr, lpSockAddrLen);
    if (hNew == INVALID_SOCKET) return FALSE;

    rConnectedSocket.Attach(hNew);
    return TRUE;
}

int CAsyncSocket::Connect(const wchar_t* lpszHostAddress, UINT nHostPort) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)nHostPort);

    if (lpszHostAddress && lpszHostAddress[0]) {
        InetPtonW(AF_INET, lpszHostAddress, &addr.sin_addr);
    }

    return ConnectHelper((const sockaddr*)&addr, sizeof(addr));
}

int CAsyncSocket::Connect(const sockaddr* lpSockAddr, int nSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ConnectHelper(lpSockAddr, nSockAddrLen);
}

int CAsyncSocket::ConnectHelper(const sockaddr* lpSockAddr, int nSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    int result = ::connect(m_hSocket, lpSockAddr, nSockAddrLen);
    if (result == SOCKET_ERROR) {
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
            // Non-blocking connect in progress
            return TRUE;
        }
        return FALSE;
    }

    m_bConnected = TRUE;
    return TRUE;
}

void CAsyncSocket::Close() {
    if (m_hSocket != INVALID_SOCKET) {
        AsyncSelect(0);
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }
    m_bConnected = FALSE;
}

int CAsyncSocket::Send(const void* lpBuf, int nBufLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::send(m_hSocket, (const char*)lpBuf, nBufLen, nFlags);
}

int CAsyncSocket::Receive(void* lpBuf, int nBufLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
}

int CAsyncSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
                          const wchar_t* lpszHostAddress, int nFlags) {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)nHostPort);
    if (lpszHostAddress) InetPtonW(AF_INET, lpszHostAddress, &addr.sin_addr);
    else addr.sin_addr.s_addr = INADDR_BROADCAST;
    return SendTo(lpBuf, nBufLen, (const sockaddr*)&addr, sizeof(addr), nFlags);
}

int CAsyncSocket::SendTo(const void* lpBuf, int nBufLen, const sockaddr* lpSockAddr,
                          int nSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::sendto(m_hSocket, (const char*)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}

int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                               UINT& rSocketPort, int nFlags) {
    sockaddr_in addr = {};
    int addrLen = sizeof(addr);
    int result = ReceiveFrom(lpBuf, nBufLen, (sockaddr*)&addr, &addrLen, nFlags);
    if (result != SOCKET_ERROR) {
        wchar_t buf[64];
        InetNtopW(AF_INET, &addr.sin_addr, buf, 64);
        rSocketAddress = buf;
        rSocketPort = ntohs(addr.sin_port);
    }
    return result;
}

int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, sockaddr* lpSockAddr,
                               int* lpSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
}

int CAsyncSocket::AsyncSelect(long lEvent) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    m_lEvent = lEvent;
    // Use WSAAsyncSelect with a hidden window? For simplicity, just set non-blocking mode
    if (lEvent != 0) {
        u_long mode = 1;
        ioctlsocket(m_hSocket, FIONBIO, &mode);
    }
    return TRUE;
}

int CAsyncSocket::IOCtl(long lCommand, DWORD* lpArgument) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ioctlsocket(m_hSocket, lCommand, lpArgument);
}

int CAsyncSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort) {
    sockaddr_in addr = {};
    int addrLen = sizeof(addr);
    int result = GetPeerName((sockaddr*)&addr, &addrLen);
    if (result) {
        wchar_t buf[64];
        InetNtopW(AF_INET, &addr.sin_addr, buf, 64);
        rPeerAddress = buf;
        rPeerPort = ntohs(addr.sin_port);
    }
    return result;
}

int CAsyncSocket::GetPeerName(sockaddr* lpSockAddr, int* lpSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::getpeername(m_hSocket, lpSockAddr, lpSockAddrLen) != SOCKET_ERROR;
}

int CAsyncSocket::GetSockName(CString& rSocketAddress, UINT& rSocketPort) {
    sockaddr_in addr = {};
    int addrLen = sizeof(addr);
    int result = GetSockName((sockaddr*)&addr, &addrLen);
    if (result) {
        wchar_t buf[64];
        InetNtopW(AF_INET, &addr.sin_addr, buf, 64);
        rSocketAddress = buf;
        rSocketPort = ntohs(addr.sin_port);
    }
    return result;
}

int CAsyncSocket::GetSockName(sockaddr* lpSockAddr, int* lpSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::getsockname(m_hSocket, lpSockAddr, lpSockAddrLen) != SOCKET_ERROR;
}

int CAsyncSocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::getsockopt(m_hSocket, nLevel, nOptionName, (char*)lpOptionValue, lpOptionLen) != SOCKET_ERROR;
}

int CAsyncSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    return ::setsockopt(m_hSocket, nLevel, nOptionName, (const char*)lpOptionValue, nOptionLen) != SOCKET_ERROR;
}

int CAsyncSocket::GetLastError() {
    return WSAGetLastError();
}

int CAsyncSocket::GetLastError(int* pnErrorCode) {
    if (pnErrorCode) *pnErrorCode = WSAGetLastError();
    return 0;
}

void CAsyncSocket::SetLastError(int nErrorCode) {
    WSASetLastError(nErrorCode);
}

void CAsyncSocket::DoCallBack(SOCKET hSocket, long lParam) {
    (void)hSocket; (void)lParam;
    // Callback mechanism - simplified
}

SOCKET CAsyncSocket::DetachHandle(SOCKET hSocket, int bDeadSocket) {
    (void)bDeadSocket;
    return hSocket;
}

void CAsyncSocket::AttachHandle(SOCKET hSocket, CAsyncSocket* pSocket, int bDeadSocket) {
    (void)hSocket; (void)pSocket; (void)bDeadSocket;
    // Attach handle mapping
}

//=============================================================================
// CSocket
//=============================================================================

IMPLEMENT_DYNAMIC(CSocket, CAsyncSocket)

CSocket::CSocket()
    : CAsyncSocket(), m_bBlocking(FALSE)
{
    memset(_socket_padding, 0, sizeof(_socket_padding));
}

CSocket::~CSocket() {
}

int CSocket::Accept(CAsyncSocket& rConnectedSocket,
                     sockaddr* lpSockAddr, int* lpSockAddrLen) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    SOCKET hNew = ::accept(m_hSocket, lpSockAddr, lpSockAddrLen);
    if (hNew == INVALID_SOCKET) {
        if (m_bBlocking) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                PumpMessages(FD_ACCEPT);
                hNew = ::accept(m_hSocket, lpSockAddr, lpSockAddrLen);
            }
        }
        if (hNew == INVALID_SOCKET) return FALSE;
    }

    rConnectedSocket.Attach(hNew);
    return TRUE;
}

void CSocket::Close() {
    m_bBlocking = FALSE;
    CAsyncSocket::Close();
}

int CSocket::Send(const void* lpBuf, int nBufLen, int nFlags) {
    return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

int CSocket::Receive(void* lpBuf, int nBufLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    int result = ::recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
    if (result == SOCKET_ERROR && m_bBlocking) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            PumpMessages(FD_READ);
            result = ::recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
        }
    }
    return result;
}

int CSocket::IsBlocking() const {
    return m_bBlocking;
}

int CSocket::CancelBlockingCall() {
    m_bBlocking = FALSE;
    return TRUE;
}

int CSocket::ConnectHelper(const sockaddr* lpSockAddr, int nSockAddrLen) {
    return CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen);
}

int CSocket::PumpMessages(UINT uStopFlag) {
    (void)uStopFlag;
    // Simplified message pump for blocking operations
    MSG msg;
    for (int i = 0; i < 100; i++) {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return TRUE;
}

//=============================================================================
// CSocketFile
//=============================================================================

CSocketFile::CSocketFile(CSocket* pSocket, int bArchiveCompatible)
    : CFile(), m_pSocket(pSocket), m_bArchiveCompatible(bArchiveCompatible)
{
    memset(_socketfile_padding, 0, sizeof(_socketfile_padding));
}

CSocketFile::~CSocketFile() {
}

UINT CSocketFile::Read(void* lpBuf, UINT nCount) {
    if (!m_pSocket) return 0;
    int result = m_pSocket->Receive(lpBuf, nCount);
    return (result == SOCKET_ERROR) ? 0 : (UINT)result;
}

void CSocketFile::Write(const void* lpBuf, UINT nCount) {
    if (m_pSocket) {
        m_pSocket->Send(lpBuf, nCount);
    }
}

void CSocketFile::Close() {
    // Don't close the socket, just the file wrapper
}

ULONGLONG CSocketFile::Seek(LONGLONG lOff, UINT nFrom) {
    (void)lOff; (void)nFrom;
    return (ULONGLONG)-1;  // Sockets don't support seek
}

//=============================================================================
// CSocket - additional methods
//=============================================================================
int CSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
                     const wchar_t* lpszHostAddress, int nFlags) {
    (void)lpBuf; (void)nBufLen; (void)nHostPort; (void)lpszHostAddress; (void)nFlags;
    return SOCKET_ERROR;
}

int CSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                          UINT& rSocketPort, int nFlags) {
    (void)lpBuf; (void)nBufLen; (void)rSocketAddress; (void)rSocketPort; (void)nFlags;
    return SOCKET_ERROR;
}

void CSocket::AuxQueueAdd(UINT message, SOCKET hSocket, long lParam) {
    (void)message; (void)hSocket; (void)lParam;
}
