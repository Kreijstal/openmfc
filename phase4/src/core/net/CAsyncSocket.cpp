// CAsyncSocket — OpenMFC implementation.
// Sources: sockcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SockcoreSupport.h"

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
    return CAsyncSocket::GetThisClass();
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
    CAsyncSocket* pSocket = LookupHandle(hSocket, FALSE);
    if (!pSocket) return;

    const int nErrorCode = WSAGETSELECTERROR(lParam);
    switch (WSAGETSELECTEVENT(lParam)) {
    case FD_READ:
        pSocket->OnReceive(nErrorCode);
        break;
    case FD_WRITE:
        pSocket->OnSend(nErrorCode);
        break;
    case FD_OOB:
        pSocket->OnOutOfBandData(nErrorCode);
        break;
    case FD_ACCEPT:
        pSocket->OnAccept(nErrorCode);
        break;
    case FD_CONNECT:
        pSocket->m_bConnected = (nErrorCode == 0);
        pSocket->OnConnect(nErrorCode);
        break;
    case FD_CLOSE:
        pSocket->m_bConnected = FALSE;
        pSocket->OnClose(nErrorCode);
        break;
    default:
        break;
    }
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

// === Moved from ManualThunks.cpp ===
// Symbol: ?Accept@CAsyncSocket@@UEAAHAEAV1@PEAUsockaddr@@PEAH@Z
extern "C" int MS_ABI impl__Accept_CAsyncSocket__UEAAHAEAV1_PEAUsockaddr__PEAH_Z(void* pThis, void* p0, void* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
    return 0;
}


// Symbol: ?AttachHandle@CAsyncSocket@@SAX_KPEAV1@H@Z
extern "C" void MS_ABI impl__AttachHandle_CAsyncSocket__SAX_KPEAV1_H_Z(void* p0, void* p1, void* p2) {
    (void)p0;
    (void)p1;
    (void)p2;
}

