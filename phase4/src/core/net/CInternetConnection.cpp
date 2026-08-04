// CInternetConnection — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ??0CInternetConnection@@QEAA@PEAVCInternetSession@@PEB_WG_K@Z
extern "C" void* MS_ABI impl___0CInternetConnection__QEAA_PEAVCInternetSession__PEB_WG_K_Z(
    void* pThis, CInternetSession* pSession, const wchar_t* pstrServer, unsigned short nPort, unsigned __int64 dwContext) {
    return new (pThis) CInternetConnection(pSession, pstrServer, (INTERNET_PORT)nPort, (DWORD_PTR)dwContext);
}
// Symbol: ?Close@CInternetConnection@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CInternetConnection__UEAAXXZ(CInternetConnection* pThis) {
    pThis->Close();
}
// Symbol: ?QueryOption@CInternetConnection@@QEBAHKPEAXPEAK@Z
extern "C" int MS_ABI impl__QueryOption_CInternetConnection__QEBAHKPEAXPEAK_Z(
    const CInternetConnection* pThis, unsigned long dwOption, void* lpBuffer, const unsigned long* pdwLenIn) {
    if (!pdwLenIn) return FALSE;
    unsigned long dwLen = *pdwLenIn;
    return pThis->QueryOption(dwOption, lpBuffer, &dwLen);
}
// Symbol: ?QueryOption@CInternetConnection@@QEBAHKAEAK@Z
extern "C" int MS_ABI impl__QueryOption_CInternetConnection__QEBAHKAEAK_Z(
    const CInternetConnection* pThis, unsigned long dwOption, unsigned long& dwValue) {
    return pThis->QueryOption(dwOption, dwValue);
}
// Symbol: ?SetOption@CInternetConnection@@QEAAHKPEAXKK@Z
extern "C" int MS_ABI impl__SetOption_CInternetConnection__QEAAHKPEAXKK_Z(
    CInternetConnection* pThis, unsigned long dwOption, void* lpBuffer, unsigned long dwBufLen, unsigned long dwReserved) {
    return pThis->SetOption(dwOption, lpBuffer, dwBufLen, dwReserved);
}
// Symbol: ?GetRuntimeClass@CInternetConnection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInternetConnection__UEBAPEAUCRuntimeClass__XZ(
    const CInternetConnection* pThis) {
    return CInternetConnection::GetThisClass();
}
// Symbol: ?GetThisClass@CInternetConnection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInternetConnection__SAPEAUCRuntimeClass__XZ() {
    return CInternetConnection::GetThisClass();
}
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
    Close();
}
void CInternetConnection::Close() {
    if (m_hConnection) {
        InternetCloseHandle(m_hConnection);
        m_hConnection = nullptr;
    }
}
int CInternetConnection::QueryOption(DWORD dwOption, void* lpBuffer, DWORD* pdwBufLen) const {
    if (!m_hConnection) return FALSE;
    return InternetQueryOptionW(m_hConnection, dwOption, lpBuffer, pdwBufLen);
}
int CInternetConnection::QueryOption(DWORD dwOption, DWORD& dwValue) const {
    DWORD dwSize = sizeof(dwValue);
    return QueryOption(dwOption, &dwValue, &dwSize);
}
int CInternetConnection::SetOption(DWORD dwOption, void* lpBuffer, DWORD dwBufLen, DWORD dwReserved) {
    (void)dwReserved;
    if (!m_hConnection) return FALSE;
    return InternetSetOptionW(m_hConnection, dwOption, lpBuffer, dwBufLen);
}
