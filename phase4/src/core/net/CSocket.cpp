// CSocket — OpenMFC implementation.
// Sources: sockcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SockcoreSupport.h"

// Symbol: ?SendChunk@CSocket@@QEAAHPEBXHH@Z
extern "C" int MS_ABI impl__SendChunk_CSocket__QEAAHPEBXHH_Z(
        CSocket* pThis, const void* lpBuf, int nBufLen, int nFlags) {
    return pThis->SendChunk(lpBuf, nBufLen, nFlags);
}
// Symbol: ?ReceiveFromHelper@CSocket@@MEAAHPEAXHPEAUsockaddr@@PEAHH@Z
extern "C" int MS_ABI impl__ReceiveFromHelper_CSocket__MEAAHPEAXHPEAUsockaddr__PEAHH_Z(
        CSocket* pThis, void* lpBuf, int nBufLen,
        sockaddr* lpSockAddr, int* lpSockAddrLen, int nFlags) {
    return pThis->_OpenMfcReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags);
}
// Symbol: ?SendToHelper@CSocket@@MEAAHPEBXHPEBUsockaddr@@HH@Z
extern "C" int MS_ABI impl__SendToHelper_CSocket__MEAAHPEBXHPEBUsockaddr__HH_Z(
        CSocket* pThis, const void* lpBuf, int nBufLen,
        const sockaddr* lpSockAddr, int nSockAddrLen, int nFlags) {
    return pThis->_OpenMfcSendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags);
}
// Symbol: ?OnMessagePending@CSocket@@MEAAHXZ
extern "C" int MS_ABI impl__OnMessagePending_CSocket__MEAAHXZ(CSocket* pThis) {
    return pThis->_OpenMfcOnMessagePending();
}
// Symbol: ?ProcessAuxQueue@CSocket@@SAHXZ
extern "C" int MS_ABI impl__ProcessAuxQueue_CSocket__SAHXZ() {
    return CSocket::ProcessAuxQueue();
}
// Symbol: ?GetRuntimeClass@CSocket@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSocket__UEBAPEAUCRuntimeClass__XZ(
        const CSocket* pThis) {
    if (!pThis) return &CSocket::classCSocket;
    return CSocket::GetThisClass();
}
// Symbol: ?GetThisClass@CSocket@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSocket__SAPEAUCRuntimeClass__XZ() {
    return CSocket::GetThisClass();
}
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
int CSocket::OnMessagePending() {
    ProcessAuxQueue();
    return PumpMessages(0);
}
int CSocket::ProcessAuxQueue() {
    std::deque<AuxSocketMessage> pending;
    {
        std::lock_guard<std::mutex> lock(g_auxQueueMutex);
        if (g_auxQueue.empty()) return TRUE;
        pending.swap(g_auxQueue);
    }

    for (const auto& item : pending) {
        if (item.message == kSocketNotifyMessage) {
            CAsyncSocket::DoCallBack(item.socket, item.lParam);
        } else if (item.message == kSocketDeadMessage) {
            constexpr int kLookupDeadSocket = TRUE;
            CAsyncSocket* pSocket = CAsyncSocket::LookupHandle(item.socket, kLookupDeadSocket);
            if (pSocket) pSocket->Close();
        }
    }
    return TRUE;
}
void CSocket::AuxQueueAdd(UINT message, SOCKET hSocket, long lParam) {
    std::lock_guard<std::mutex> lock(g_auxQueueMutex);
    g_auxQueue.push_back({message, hSocket, lParam});
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
