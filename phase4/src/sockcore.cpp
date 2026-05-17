// Socket Implementation
// Provides MFC wrappers around WinSock2 API
//
// All exports use MSVC ABI via extern "C" + .def aliasing.
// impl_xxx functions are the exported names as mapped in openmfc.def.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxsock.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <map>
#include <mutex>

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

// Simple socket handle map for LookupHandle/KillSocket/AttachHandle
static std::map<SOCKET, CAsyncSocket*> g_socketMap;
static std::mutex g_socketMapMutex;

//=============================================================================
// AfxSocketInit (existing public export - symbol already in thunks.cpp)
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

// Symbol: ?AfxSocketTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxSocketTerm__YAXXZ() {
    if (g_bSocketsInitialized) {
        WSACleanup();
        g_bSocketsInitialized = FALSE;
    }
}

// Symbol: ?_AfxSocketInit@@YAHPEAUWSAData@@@Z
extern "C" int MS_ABI impl___AfxSocketInit__YAHPEAUWSAData___Z(WSADATA* lpwsaData) {
    return AfxSocketInit(lpwsaData);
}

//=============================================================================
// CAsyncSocket C++ implementation
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
    if (m_hSocket != INVALID_SOCKET) return FALSE;

    m_hSocket = socket(AF_INET, nSocketType, 0);
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    m_nSocketType = nSocketType;
    m_lEvent = lEvent;

    if (nSocketPort != 0 || lpszSocketAddress) {
        if (!Bind(nSocketPort, lpszSocketAddress)) {
            Close();
            return FALSE;
        }
    }

    if (lEvent != 0 && lEvent != -1) {
        AsyncSelect(lEvent);
    }

    return TRUE;
}

SOCKET CAsyncSocket::Detach() {
    SOCKET h = m_hSocket;
    if (h != INVALID_SOCKET) {
        AsyncSelect(0);
        KillSocket(h, this);
    }
    m_hSocket = INVALID_SOCKET;
    m_bConnected = FALSE;
    return h;
}

int CAsyncSocket::Attach(SOCKET hSocket, long lEvent) {
    if (m_hSocket != INVALID_SOCKET) return FALSE;
    if (hSocket == INVALID_SOCKET) return FALSE;

    m_hSocket = hSocket;
    m_lEvent = lEvent;

    AttachHandle(hSocket, this, FALSE);

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
        KillSocket(m_hSocket, this);
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
    return SendToHelper(lpBuf, nBufLen, (const sockaddr*)&addr, sizeof(addr), nFlags);
}

int CAsyncSocket::SendTo(const void* lpBuf, int nBufLen, const sockaddr* lpSockAddr,
                          int nSockAddrLen, int nFlags) {
    return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
}

int CAsyncSocket::SendToHelper(const void* lpBuf, int nBufLen, const sockaddr* lpSockAddr,
                                int nSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::sendto(m_hSocket, (const char*)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}

int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                               UINT& rSocketPort, int nFlags) {
    sockaddr_in addr = {};
    int addrLen = sizeof(addr);
    int result = ReceiveFromHelper(lpBuf, nBufLen, (sockaddr*)&addr, &addrLen, nFlags);
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
    return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
}

int CAsyncSocket::ReceiveFromHelper(void* lpBuf, int nBufLen, sockaddr* lpSockAddr,
                                     int* lpSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
}

int CAsyncSocket::AsyncSelect(long lEvent) {
    if (m_hSocket == INVALID_SOCKET) return FALSE;
    m_lEvent = lEvent;
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
}

SOCKET CAsyncSocket::DetachHandle(SOCKET hSocket, int bDeadSocket) {
    (void)bDeadSocket;
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    g_socketMap.erase(hSocket);
    return hSocket;
}

void CAsyncSocket::AttachHandle(SOCKET hSocket, CAsyncSocket* pSocket, int bDeadSocket) {
    (void)bDeadSocket;
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (pSocket) {
        g_socketMap[hSocket] = pSocket;
    } else {
        g_socketMap.erase(hSocket);
    }
}

CAsyncSocket* CAsyncSocket::LookupHandle(SOCKET hSocket, int bDeadSocket) {
    (void)bDeadSocket;
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    auto it = g_socketMap.find(hSocket);
    if (it != g_socketMap.end()) {
        return it->second;
    }
    return nullptr;
}

void CAsyncSocket::KillSocket(SOCKET hSocket, CAsyncSocket* pSocket) {
    (void)pSocket;
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    g_socketMap.erase(hSocket);
}

int CAsyncSocket::Socket(int nSocketType, long lEvent, int nProtocolType, int nAddressFormat) {
    if (m_hSocket != INVALID_SOCKET) return FALSE;

    m_hSocket = socket(nAddressFormat, nSocketType, nProtocolType);
    if (m_hSocket == INVALID_SOCKET) return FALSE;

    m_nSocketType = nSocketType;
    m_lEvent = lEvent;

    if (lEvent != 0 && lEvent != -1) {
        AsyncSelect(lEvent);
    }
    return TRUE;
}

void CAsyncSocket::OnAccept(int nErrorCode) { (void)nErrorCode; }
void CAsyncSocket::OnClose(int nErrorCode) { (void)nErrorCode; }
void CAsyncSocket::OnConnect(int nErrorCode) { (void)nErrorCode; }
void CAsyncSocket::OnOutOfBandData(int nErrorCode) { (void)nErrorCode; }
void CAsyncSocket::OnReceive(int nErrorCode) { (void)nErrorCode; }
void CAsyncSocket::OnSend(int nErrorCode) { (void)nErrorCode; }

//=============================================================================
// CSocket C++ implementation
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
        if (m_bBlocking && WSAGetLastError() == WSAEWOULDBLOCK) {
            PumpMessages(FD_ACCEPT);
            hNew = ::accept(m_hSocket, lpSockAddr, lpSockAddrLen);
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
    return SendChunk(lpBuf, nBufLen, nFlags);
}

int CSocket::Receive(void* lpBuf, int nBufLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    int result = ::recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
    if (result == SOCKET_ERROR && m_bBlocking && WSAGetLastError() == WSAEWOULDBLOCK) {
        PumpMessages(FD_READ);
        result = ::recv(m_hSocket, (char*)lpBuf, nBufLen, nFlags);
    }
    return result;
}

int CSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort,
                     const wchar_t* lpszHostAddress, int nFlags) {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)nHostPort);
    if (lpszHostAddress) InetPtonW(AF_INET, lpszHostAddress, &addr.sin_addr);
    else addr.sin_addr.s_addr = INADDR_BROADCAST;
    return SendToHelper(lpBuf, nBufLen, (const sockaddr*)&addr, sizeof(addr), nFlags);
}

int CSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress,
                          UINT& rSocketPort, int nFlags) {
    sockaddr_in addr = {};
    int addrLen = sizeof(addr);
    int result = ReceiveFromHelper(lpBuf, nBufLen, (sockaddr*)&addr, &addrLen, nFlags);
    if (result != SOCKET_ERROR) {
        wchar_t buf[64];
        InetNtopW(AF_INET, &addr.sin_addr, buf, 64);
        rSocketAddress = buf;
        rSocketPort = ntohs(addr.sin_port);
    }
    return result;
}

int CSocket::IsBlocking() const { return m_bBlocking; }

int CSocket::CancelBlockingCall() {
    m_bBlocking = FALSE;
    return TRUE;
}

int CSocket::ConnectHelper(const sockaddr* lpSockAddr, int nSockAddrLen) {
    return CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen);
}

int CSocket::SendChunk(const void* lpBuf, int nBufLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    int nLeft = nBufLen;
    const char* pBuf = (const char*)lpBuf;
    while (nLeft > 0) {
        int nSent = ::send(m_hSocket, pBuf, nLeft, nFlags);
        if (nSent == SOCKET_ERROR) {
            if (m_bBlocking && WSAGetLastError() == WSAEWOULDBLOCK) {
                PumpMessages(FD_WRITE);
                continue;
            }
            return SOCKET_ERROR;
        }
        pBuf += nSent;
        nLeft -= nSent;
    }
    return nBufLen;
}

int CSocket::ReceiveFromHelper(void* lpBuf, int nBufLen, sockaddr* lpSockAddr,
                                int* lpSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    int result = ::recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
    if (result == SOCKET_ERROR && m_bBlocking && WSAGetLastError() == WSAEWOULDBLOCK) {
        PumpMessages(FD_READ);
        result = ::recvfrom(m_hSocket, (char*)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen);
    }
    return result;
}

int CSocket::SendToHelper(const void* lpBuf, int nBufLen, const sockaddr* lpSockAddr,
                           int nSockAddrLen, int nFlags) {
    if (m_hSocket == INVALID_SOCKET) return SOCKET_ERROR;
    return ::sendto(m_hSocket, (const char*)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen);
}

int CSocket::OnMessagePending() { return FALSE; }
int CSocket::ProcessAuxQueue() { return TRUE; }
void CSocket::AuxQueueAdd(UINT message, SOCKET hSocket, long lParam) {
    (void)message; (void)hSocket; (void)lParam;
}

int CSocket::PumpMessages(UINT uStopFlag) {
    (void)uStopFlag;
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
// CSocketFile C++ implementation
//=============================================================================

// Manual CRuntimeClass definition (CFile doesn't derive from CObject)
CRuntimeClass CSocketFile::classCSocketFile = {
    "CSocketFile",
    sizeof(CSocketFile),
    0xFFFF,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

CSocketFile::CSocketFile(CSocket* pSocket, int bArchiveCompatible)
    : CFile(), m_pSocket(pSocket), m_bArchiveCompatible(bArchiveCompatible)
{
    memset(_socketfile_padding, 0, sizeof(_socketfile_padding));
}

CSocketFile::~CSocketFile() {
}

UINT CSocketFile::Read(void* lpBuf, UINT nCount) {
    if (!m_pSocket) return 0;
    int result = m_pSocket->Receive(lpBuf, (int)nCount);
    return (result == SOCKET_ERROR) ? 0 : (UINT)result;
}

void CSocketFile::Write(const void* lpBuf, UINT nCount) {
    if (m_pSocket) {
        m_pSocket->Send(lpBuf, (int)nCount);
    }
}

void CSocketFile::Close() {
    // Detach only - don't close the underlying socket
}

ULONGLONG CSocketFile::Seek(LONGLONG lOff, UINT nFrom) {
    (void)lOff; (void)nFrom;
    return (ULONGLONG)-1;
}

void CSocketFile::Flush() { /* no-op: no buffering */ }

void CSocketFile::Abort() {
    if (m_pSocket) {
        m_pSocket->Close();
    }
}

ULONGLONG CSocketFile::GetLength() const { return (ULONGLONG)-1; }
ULONGLONG CSocketFile::GetPosition() const { return (ULONGLONG)-1; }
void CSocketFile::SetLength(ULONGLONG dwNewLen) { (void)dwNewLen; }
CFile* CSocketFile::Duplicate() const { return nullptr; }

UINT CSocketFile::GetBufferPtr(UINT nCommand, UINT nCount, void** ppBufStart, void** ppBufMax) {
    (void)nCommand; (void)nCount;
    if (ppBufStart) *ppBufStart = nullptr;
    if (ppBufMax) *ppBufMax = nullptr;
    return 0;
}

void CSocketFile::LockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos; (void)dwCount;
}

void CSocketFile::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos; (void)dwCount;
}

int CSocketFile::Open(const wchar_t* lpszFileName, UINT nOpenFlags, CFileException* pError) {
    (void)lpszFileName; (void)nOpenFlags; (void)pError;
    return FALSE;
}

//=============================================================================
// CSocketWnd C++ implementation
//=============================================================================

#define WM_SOCKET_NOTIFY  (WM_USER + 0)
#define WM_SOCKET_DEAD    (WM_USER + 1)

BEGIN_MESSAGE_MAP(CSocketWnd, CWnd)
END_MESSAGE_MAP()

CSocketWnd::CSocketWnd() : CWnd() {
}

LRESULT CSocketWnd::OnSocketNotify(WPARAM wParam, LPARAM lParam) {
    SOCKET hSocket = (SOCKET)wParam;
    long lEvent = (long)lParam;
    CAsyncSocket* pSocket = CAsyncSocket::LookupHandle(hSocket, FALSE);
    if (pSocket) {
        CAsyncSocket::DoCallBack(hSocket, lEvent);
    }
    return 0;
}

LRESULT CSocketWnd::OnSocketDead(WPARAM wParam, LPARAM lParam) {
    SOCKET hSocket = (SOCKET)wParam;
    (void)lParam;
    CAsyncSocket* pSocket = CAsyncSocket::LookupHandle(hSocket, TRUE);
    if (pSocket) {
        pSocket->Close();
    }
    return 0;
}

//=============================================================================
// impl_xxx Export Wrappers
// The .def file maps MSVC_NAME=impl_xxx for each symbol below.
// Symbols marked with "// Symbol:" are counted as real by gen_manifest.py.
//=============================================================================

// Symbol: ?Socket@CAsyncSocket@@QEAAHHJHH@Z
extern "C" int MS_ABI impl__Socket_CAsyncSocket__QEAAHHJHH_Z(
        CAsyncSocket* pThis, int nSocketType, long lEvent, int nProtocolType, int nAddressFormat) {
    return pThis->Socket(nSocketType, lEvent, nProtocolType, nAddressFormat);
}

// Symbol: ?LookupHandle@CAsyncSocket@@SAPEAV1@_KH@Z
extern "C" CAsyncSocket* MS_ABI impl__LookupHandle_CAsyncSocket__SAPEAV1__KH_Z(
        SOCKET hSocket, int bDeadSocket) {
    return CAsyncSocket::LookupHandle(hSocket, bDeadSocket);
}

// Symbol: ?KillSocket@CAsyncSocket@@SAX_KPEAV1@@Z
extern "C" void MS_ABI impl__KillSocket_CAsyncSocket__SAX_KPEAV1__Z(
        SOCKET hSocket, CAsyncSocket* pSocket) {
    CAsyncSocket::KillSocket(hSocket, pSocket);
}

// Symbol: ?OnAccept@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnAccept_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnAccept(nErrorCode);
}

// Symbol: ?OnClose@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnClose_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnClose(nErrorCode);
}

// Symbol: ?OnConnect@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnConnect_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnConnect(nErrorCode);
}

// Symbol: ?OnOutOfBandData@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnOutOfBandData_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnOutOfBandData(nErrorCode);
}

// Symbol: ?OnReceive@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnReceive_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnReceive(nErrorCode);
}

// Symbol: ?OnSend@CAsyncSocket@@MEAAXH@Z
extern "C" void MS_ABI impl__OnSend_CAsyncSocket__MEAAXH_Z(
        CAsyncSocket* pThis, int nErrorCode) {
    pThis->OnSend(nErrorCode);
}

// Symbol: ?ReceiveFromHelper@CAsyncSocket@@MEAAHPEAXHPEAUsockaddr@@PEAHH@Z
extern "C" int MS_ABI impl__ReceiveFromHelper_CAsyncSocket__MEAAHPEAXHPEAUsockaddr__PEAHH_Z(
        CAsyncSocket* pThis, void* lpBuf, int nBufLen,
        sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags) {
    return pThis->ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
}

// Symbol: ?SendToHelper@CAsyncSocket@@MEAAHPEBXHPEBUsockaddr@@HH@Z
extern "C" int MS_ABI impl__SendToHelper_CAsyncSocket__MEAAHPEBXHPEBUsockaddr__HH_Z(
        CAsyncSocket* pThis, const void* lpBuf, int nBufLen,
        const sockaddr* lpSockAddr, int nSockAddrLen, int nFlags) {
    return pThis->SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
}

// Symbol: ?GetRuntimeClass@CAsyncSocket@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAsyncSocket__UEBAPEAUCRuntimeClass__XZ(
        const CAsyncSocket* pThis) {
    if (!pThis) return &CAsyncSocket::classCAsyncSocket;
    return pThis->GetRuntimeClass();
}

// Symbol: ?GetThisClass@CAsyncSocket@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAsyncSocket__SAPEAUCRuntimeClass__XZ() {
    return CAsyncSocket::GetThisClass();
}

// Symbol: ?GetPeerName@CAsyncSocket@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAI@Z
extern "C" int MS_ABI impl__GetPeerName_CAsyncSocket__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAI_Z(
        CAsyncSocket* pThis, CString* rPeerAddress, UINT* rPeerPort) {
    if (!pThis || !rPeerAddress || !rPeerPort) return FALSE;
    return pThis->GetPeerName(*rPeerAddress, *rPeerPort);
}

// Symbol: ?GetSockName@CAsyncSocket@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAI@Z
extern "C" int MS_ABI impl__GetSockName_CAsyncSocket__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAI_Z(
        CAsyncSocket* pThis, CString* rSocketAddress, UINT* rSocketPort) {
    if (!pThis || !rSocketAddress || !rSocketPort) return FALSE;
    return pThis->GetSockName(*rSocketAddress, *rSocketPort);
}

// Symbol: ?ReceiveFrom@CAsyncSocket@@QEAAHPEAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAIH@Z
extern "C" int MS_ABI impl__ReceiveFrom_CAsyncSocket__QEAAHPEAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAIH_Z(
        CAsyncSocket* pThis, void* lpBuf, int nBufLen,
        CString* rSocketAddress, UINT* rSocketPort, int nFlags) {
    if (!pThis || !rSocketAddress || !rSocketPort) return SOCKET_ERROR;
    return pThis->ReceiveFrom(lpBuf, nBufLen, *rSocketAddress, *rSocketPort, nFlags);
}

// Symbol: ?SendChunk@CSocket@@QEAAHPEBXHH@Z
extern "C" int MS_ABI impl__SendChunk_CSocket__QEAAHPEBXHH_Z(
        CSocket* pThis, const void* lpBuf, int nBufLen, int nFlags) {
    return pThis->SendChunk(lpBuf, nBufLen, nFlags);
}

// Symbol: ?ReceiveFromHelper@CSocket@@MEAAHPEAXHPEAUsockaddr@@PEAHH@Z
extern "C" int MS_ABI impl__ReceiveFromHelper_CSocket__MEAAHPEAXHPEAUsockaddr__PEAHH_Z(
        CSocket* pThis, void* lpBuf, int nBufLen,
        sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags) {
    return pThis->ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
}

// Symbol: ?SendToHelper@CSocket@@MEAAHPEBXHPEBUsockaddr@@HH@Z
extern "C" int MS_ABI impl__SendToHelper_CSocket__MEAAHPEBXHPEBUsockaddr__HH_Z(
        CSocket* pThis, const void* lpBuf, int nBufLen,
        const sockaddr* lpSockAddr, int nSockAddrLen, int nFlags) {
    return pThis->SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
}

// Symbol: ?OnMessagePending@CSocket@@MEAAHXZ
extern "C" int MS_ABI impl__OnMessagePending_CSocket__MEAAHXZ(CSocket* pThis) {
    return pThis->OnMessagePending();
}

// Symbol: ?ProcessAuxQueue@CSocket@@SAHXZ
extern "C" int MS_ABI impl__ProcessAuxQueue_CSocket__SAHXZ() {
    return CSocket::ProcessAuxQueue();
}

// Symbol: ?GetRuntimeClass@CSocket@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSocket__UEBAPEAUCRuntimeClass__XZ(
        const CSocket* pThis) {
    if (!pThis) return &CSocket::classCSocket;
    return pThis->GetRuntimeClass();
}

// Symbol: ?GetThisClass@CSocket@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSocket__SAPEAUCRuntimeClass__XZ() {
    return CSocket::GetThisClass();
}

// Symbol: ??0CSocketFile@@QEAA@PEAVCSocket@@H@Z
extern "C" void* MS_ABI impl___0CSocketFile__QEAA_PEAVCSocket__H_Z(
        CSocketFile* pThis, CSocket* pSocket, int bArchiveCompatible) {
    new(pThis) CSocketFile(pSocket, bArchiveCompatible);
    return pThis;
}

// Symbol: ?Flush@CSocketFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CSocketFile__UEAAXXZ(CSocketFile* pThis) {
    pThis->Flush();
}

// Symbol: ?Abort@CSocketFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CSocketFile__UEAAXXZ(CSocketFile* pThis) {
    pThis->Abort();
}

// Symbol: ?GetLength@CSocketFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetLength_CSocketFile__UEBA_KXZ(
        const CSocketFile* pThis) {
    return pThis->GetLength();
}

// Symbol: ?GetPosition@CSocketFile@@UEBA_KXZ
extern "C" unsigned long long MS_ABI impl__GetPosition_CSocketFile__UEBA_KXZ(
        const CSocketFile* pThis) {
    return pThis->GetPosition();
}

// Symbol: ?SetLength@CSocketFile@@UEAAX_K@Z
extern "C" void MS_ABI impl__SetLength_CSocketFile__UEAAX_K_Z(
        CSocketFile* pThis, unsigned long long dwNewLen) {
    pThis->SetLength(dwNewLen);
}

// Symbol: ?Duplicate@CSocketFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CSocketFile__UEBAPEAVCFile__XZ(
        const CSocketFile* pThis) {
    return pThis->Duplicate();
}

// Symbol: ?GetBufferPtr@CSocketFile@@UEAAIIIPEAPEAX0@Z
extern "C" unsigned int MS_ABI impl__GetBufferPtr_CSocketFile__UEAAIIIPEAPEAX0_Z(
        CSocketFile* pThis, unsigned int nCommand, unsigned int nCount,
        void** ppBufStart, void** ppBufMax) {
    return pThis->GetBufferPtr(nCommand, nCount, ppBufStart, ppBufMax);
}

// Symbol: ?LockRange@CSocketFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CSocketFile__UEAAX_K0_Z(
        CSocketFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    pThis->LockRange(dwPos, dwCount);
}

// Symbol: ?UnlockRange@CSocketFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CSocketFile__UEAAX_K0_Z(
        CSocketFile* pThis, unsigned long long dwPos, unsigned long long dwCount) {
    pThis->UnlockRange(dwPos, dwCount);
}

// Symbol: ?Open@CSocketFile@@UEAAHPEB_WIPEAVCFileException@@@Z
extern "C" int MS_ABI impl__Open_CSocketFile__UEAAHPEB_WIPEAVCFileException___Z(
        CSocketFile* pThis, const wchar_t* lpszFileName,
        unsigned int nOpenFlags, CFileException* pError) {
    return pThis->Open(lpszFileName, nOpenFlags, pError);
}

// Symbol: ?GetRuntimeClass@CSocketFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSocketFile__UEBAPEAUCRuntimeClass__XZ(
        const CSocketFile* pThis) {
    if (!pThis) return &CSocketFile::classCSocketFile;
    return pThis->GetRuntimeClass();
}

// Symbol: ?GetThisClass@CSocketFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSocketFile__SAPEAUCRuntimeClass__XZ() {
    return CSocketFile::GetThisClass();
}

// Symbol: ??0CSocketWnd@@QEAA@XZ
extern "C" void* MS_ABI impl___0CSocketWnd__QEAA_XZ(CSocketWnd* pThis) {
    new(pThis) CSocketWnd();
    return pThis;
}

// Symbol: ?GetMessageMap@CSocketWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSocketWnd__MEBAPEBUAFX_MSGMAP__XZ(
        const CSocketWnd* pThis) {
    return pThis->GetMessageMap();
}

// Symbol: ?GetThisMessageMap@CSocketWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSocketWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CSocketWnd::GetThisMessageMap();
}

// Symbol: ?OnSocketNotify@CSocketWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSocketNotify_CSocketWnd__IEAA_J_K_J_Z(
        CSocketWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis->OnSocketNotify(wParam, lParam);
}

// Symbol: ?OnSocketDead@CSocketWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSocketDead_CSocketWnd__IEAA_J_K_J_Z(
        CSocketWnd* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis->OnSocketDead(wParam, lParam);
}
