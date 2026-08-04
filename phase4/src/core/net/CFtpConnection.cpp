// CFtpConnection — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ??0CFtpConnection@@QEAA@PEAVCInternetSession@@PEAXPEB_W_K@Z
extern "C" void* MS_ABI impl___0CFtpConnection__QEAA_PEAVCInternetSession__PEAXPEB_W_K_Z(
    void* pThis, CInternetSession* pSession, void* hConnected, const wchar_t* pstrServer, unsigned __int64 dwContext) {
    return new (pThis) CFtpConnection(pSession, (HINTERNET)hConnected, pstrServer, (DWORD_PTR)dwContext);
}
// Symbol: ??0CFtpConnection@@QEAA@PEAVCInternetSession@@PEB_W11_KGH@Z
extern "C" void* MS_ABI impl___0CFtpConnection__QEAA_PEAVCInternetSession__PEB_W11_KGH_Z(
    void* pThis, CInternetSession* pSession, const wchar_t* pstrServer,
    const wchar_t* pstrUserName, const wchar_t* pstrPassword,
    unsigned __int64 dwContext, unsigned short nPort, int bPassive) {
    return new (pThis) CFtpConnection(pSession, pstrServer, pstrUserName, pstrPassword,
                                      (DWORD_PTR)dwContext, (INTERNET_PORT)nPort, bPassive);
}
// Symbol: ?GetRuntimeClass@CFtpConnection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFtpConnection__UEBAPEAUCRuntimeClass__XZ(
    const CFtpConnection* pThis) {
    return CFtpConnection::GetThisClass();
}
// Symbol: ?GetThisClass@CFtpConnection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFtpConnection__SAPEAUCRuntimeClass__XZ() {
    return CFtpConnection::GetThisClass();
}
// Symbol: ?CreateDirectoryW@CFtpConnection@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__CreateDirectoryW_CFtpConnection__QEAAHPEB_W_Z(
    CFtpConnection* pThis, const wchar_t* pstrDirName) {
    return pThis->CreateDirectoryW(pstrDirName);
}
// Symbol: ?RemoveDirectoryW@CFtpConnection@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__RemoveDirectoryW_CFtpConnection__QEAAHPEB_W_Z(
    CFtpConnection* pThis, const wchar_t* pstrDirName) {
    return pThis->RemoveDirectoryW(pstrDirName);
}
// Symbol: ?SetCurrentDirectoryW@CFtpConnection@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SetCurrentDirectoryW_CFtpConnection__QEAAHPEB_W_Z(
    CFtpConnection* pThis, const wchar_t* pstrDirName) {
    return pThis->SetCurrentDirectoryW(pstrDirName);
}
// Symbol: ?GetCurrentDirectoryW@CFtpConnection@@QEBAHPEA_WPEAK@Z
extern "C" int MS_ABI impl__GetCurrentDirectoryW_CFtpConnection__QEBAHPEA_WPEAK_Z(
    const CFtpConnection* pThis, wchar_t* pstrDirName, const unsigned long* pdwLenIn) {
    if (!pdwLenIn) return FALSE;
    unsigned long len = *pdwLenIn;
    return pThis->GetCurrentDirectoryW(pstrDirName, &len);
}
// Symbol: ?GetCurrentDirectoryW@CFtpConnection@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetCurrentDirectoryW_CFtpConnection__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CFtpConnection* pThis, CString& strDirName) {
    return pThis->GetCurrentDirectoryW(strDirName);
}
// Symbol: ?GetCurrentDirectoryAsURL@CFtpConnection@@QEBAHPEA_WPEAK@Z
extern "C" int MS_ABI impl__GetCurrentDirectoryAsURL_CFtpConnection__QEBAHPEA_WPEAK_Z(
    const CFtpConnection* pThis, wchar_t* pstrDirName, const unsigned long* pdwLenIn) {
    if (!pdwLenIn) return FALSE;
    unsigned long len = *pdwLenIn;
    return pThis->GetCurrentDirectoryAsURL(pstrDirName, &len);
}
// Symbol: ?GetCurrentDirectoryAsURL@CFtpConnection@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetCurrentDirectoryAsURL_CFtpConnection__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CFtpConnection* pThis, CString& strDirName) {
    return pThis->GetCurrentDirectoryAsURL(strDirName);
}
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
int CFtpConnection::SetCurrentDirectoryW(const wchar_t* pstrDirName) {
    return SetCurrentDirectory(pstrDirName);
}
int CFtpConnection::CreateDirectory(const wchar_t* pstrDirName) {
    if (!m_hConnection) return FALSE;
    return FtpCreateDirectoryW(m_hConnection, pstrDirName);
}
int CFtpConnection::CreateDirectoryW(const wchar_t* pstrDirName) {
    return CreateDirectory(pstrDirName);
}
int CFtpConnection::RemoveDirectory(const wchar_t* pstrDirName) {
    if (!m_hConnection) return FALSE;
    return FtpRemoveDirectoryW(m_hConnection, pstrDirName);
}
int CFtpConnection::RemoveDirectoryW(const wchar_t* pstrDirName) {
    return RemoveDirectory(pstrDirName);
}
int CFtpConnection::GetCurrentDirectoryW(CString& strDirName) const {
    return GetCurrentDirectory(strDirName);
}
int CFtpConnection::GetCurrentDirectoryW(wchar_t* pstrDirName, DWORD* pdwLen) const {
    return GetCurrentDirectory(pstrDirName, pdwLen);
}
int CFtpConnection::GetCurrentDirectoryAsURL(CString& strDirName) const {
    CString dir;
    if (!GetCurrentDirectory(dir)) {
        strDirName.Empty();
        return FALSE;
    }
    strDirName = L"ftp://";
    strDirName += GetServerName();
    if (!dir.IsEmpty() && dir[0] != L'/') {
        strDirName += L"/";
    }
    strDirName += dir;
    return TRUE;
}
int CFtpConnection::GetCurrentDirectoryAsURL(wchar_t* pstrDirName, DWORD* pdwLen) const {
    if (!pstrDirName || !pdwLen || *pdwLen == 0) return FALSE;
    CString strDirName;
    if (!GetCurrentDirectoryAsURL(strDirName)) return FALSE;
    DWORD needed = (DWORD)strDirName.GetLength() + 1;
    if (*pdwLen < needed) {
        *pdwLen = needed;
        return FALSE;
    }
    wmemcpy(pstrDirName, (const wchar_t*)strDirName, needed);
    *pdwLen = needed;
    return TRUE;
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
int CFtpConnection::GetCurrentDirectory(wchar_t* pstrDirName, DWORD* pdwLen) const {
    if (!m_hConnection || !pstrDirName || !pdwLen) return 0;
    return ::FtpGetCurrentDirectoryW(m_hConnection, pstrDirName, pdwLen) ? 1 : 0;
}
CInternetFile* CFtpConnection::Command(const wchar_t* pstrCommand, CmdResponseType eResponse,
                                        CmdResponseType eResponse2, unsigned long dwContext,
                                        unsigned __int64 dwFlags) {
    (void)eResponse2;
    if (!m_hConnection || !pstrCommand) return nullptr;
    HINTERNET hResponse = nullptr;
    const BOOL expectResponse = (eResponse != CmdRespNone) ? TRUE : FALSE;
    if (!::FtpCommandW(m_hConnection, expectResponse, (DWORD)dwFlags, pstrCommand, dwContext, &hResponse)) {
        return nullptr;
    }
    if (!expectResponse || !hResponse) {
        if (hResponse) {
            ::InternetCloseHandle(hResponse);
        }
        return nullptr;
    }
    return new CInternetFile(hResponse, nullptr, this, 0);
}
int CFtpConnection::Command(const wchar_t* pstrCommand, DWORD dwCmdResponse,
                             DWORD_PTR dwContext) {
    if (!m_hConnection || !pstrCommand) return FALSE;
    return ::FtpCommandW(m_hConnection, FALSE, dwCmdResponse, pstrCommand, dwContext, nullptr);
}
