// CInternetFile — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ??0CInternetFile@@IEAA@PEAXPEB_WPEAVCInternetConnection@@H@Z
extern "C" void* MS_ABI impl___0CInternetFile__IEAA_PEAXPEB_WPEAVCInternetConnection__H_Z(
    void* pThis, void* hFile, const wchar_t* pstrFileName, CInternetConnection* pConnection, int nErrorCode) {
    return new (pThis) CInternetFile((HINTERNET)hFile, pstrFileName, pConnection, nErrorCode);
}
// Symbol: ?Abort@CInternetFile@@UEAAXXZ
extern "C" void MS_ABI impl__Abort_CInternetFile__UEAAXXZ(CInternetFile* pThis) {
    pThis->Abort();
}
// Symbol: ?Close@CInternetFile@@UEAAXXZ
extern "C" void MS_ABI impl__Close_CInternetFile__UEAAXXZ(CInternetFile* pThis) {
    pThis->Close();
}
// Symbol: ?Duplicate@CInternetFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* MS_ABI impl__Duplicate_CInternetFile__UEBAPEAVCFile__XZ(const CInternetFile* pThis) {
    return pThis->Duplicate();
}
// Symbol: ?Flush@CInternetFile@@UEAAXXZ
extern "C" void MS_ABI impl__Flush_CInternetFile__UEAAXXZ(CInternetFile* pThis) {
    pThis->Flush();
}
// Symbol: ?LockRange@CInternetFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__LockRange_CInternetFile__UEAAX_K0_Z(
    CInternetFile* pThis, unsigned __int64 dwPos, unsigned __int64 dwCount) {
    pThis->LockRange(dwPos, dwCount);
}
// Symbol: ?UnlockRange@CInternetFile@@UEAAX_K0@Z
extern "C" void MS_ABI impl__UnlockRange_CInternetFile__UEAAX_K0_Z(
    CInternetFile* pThis, unsigned __int64 dwPos, unsigned __int64 dwCount) {
    pThis->UnlockRange(dwPos, dwCount);
}
// Symbol: ?SetLength@CInternetFile@@UEAAX_K@Z
extern "C" void MS_ABI impl__SetLength_CInternetFile__UEAAX_K_Z(CInternetFile* pThis, unsigned __int64 dwNewLen) {
    pThis->SetLength(dwNewLen);
}
// Symbol: ?Write@CInternetFile@@UEAAXPEBXI@Z
extern "C" void MS_ABI impl__Write_CInternetFile__UEAAXPEBXI_Z(
    CInternetFile* pThis, const void* lpBuf, unsigned int nCount) {
    pThis->Write(lpBuf, nCount);
}
// Symbol: ?WriteString@CInternetFile@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__WriteString_CInternetFile__UEAAXPEB_W_Z(CInternetFile* pThis, const wchar_t* pstr) {
    pThis->WriteString(pstr);
}
// Symbol: ?ReadString@CInternetFile@@UEAAPEA_WPEA_WI@Z
extern "C" wchar_t* MS_ABI impl__ReadString_CInternetFile__UEAAPEA_WPEA_WI_Z(
    CInternetFile* pThis, wchar_t* pstr, unsigned int nMax) {
    return pThis->ReadString(pstr, nMax);
}
// Symbol: ?ReadString@CInternetFile@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__ReadString_CInternetFile__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CInternetFile* pThis, CString& str) {
    return pThis->ReadString(str);
}
// Symbol: ?SetOption@CInternetFile@@QEAAHKPEAXKK@Z
extern "C" int MS_ABI impl__SetOption_CInternetFile__QEAAHKPEAXKK_Z(
    CInternetFile* pThis, unsigned long dwOption, void* lpBuffer, unsigned long dwBufLen, unsigned long dwReserved) {
    return pThis->SetOption(dwOption, lpBuffer, dwBufLen, dwReserved);
}
// Symbol: ?QueryOption@CInternetFile@@QEBAHKPEAXPEAK@Z
extern "C" int MS_ABI impl__QueryOption_CInternetFile__QEBAHKPEAXPEAK_Z(
    const CInternetFile* pThis, unsigned long dwOption, void* lpBuffer, const unsigned long* pdwLenIn) {
    if (!pdwLenIn) return FALSE;
    unsigned long dwLen = *pdwLenIn;
    return pThis->QueryOption(dwOption, lpBuffer, &dwLen);
}
// Symbol: ?QueryOption@CInternetFile@@QEBAHKAEAK@Z
extern "C" int MS_ABI impl__QueryOption_CInternetFile__QEBAHKAEAK_Z(
    const CInternetFile* pThis, unsigned long dwOption, unsigned long& dwValue) {
    return pThis->QueryOption(dwOption, dwValue);
}
// Symbol: ?GetRuntimeClass@CInternetFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInternetFile__UEBAPEAUCRuntimeClass__XZ(
    const CInternetFile* pThis) {
    (void)pThis;
    return &g_classCInternetFile;
}
// Symbol: ?GetThisClass@CInternetFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInternetFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCInternetFile;
}
CInternetFile::CInternetFile()
    : CStdioFile(), m_hFile(nullptr), m_dwContext(0)
{
    memset(_inetfile_padding, 0, sizeof(_inetfile_padding));
}
CInternetFile::CInternetFile(HINTERNET hFile, const wchar_t* pstrFileName,
                             CInternetConnection* pConnection, int nErrorCode)
    : CStdioFile(), m_hFile(hFile), m_dwContext(0)
{
    (void)nErrorCode;
    if (pstrFileName) {
        m_strFileName = pstrFileName;
    }
    if (pConnection) {
        m_dwContext = pConnection->m_dwContext;
    }
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
void CInternetFile::Write(const void* lpBuf, UINT nCount) {
    if (!m_hFile || !lpBuf) return;
    DWORD dwWritten = 0;
    InternetWriteFile(m_hFile, lpBuf, nCount, &dwWritten);
}
void CInternetFile::SetLength(ULONGLONG dwNewLen) {
    (void)dwNewLen;
}
void CInternetFile::Flush() {
}
void CInternetFile::Close() {
    if (m_hFile) {
        InternetCloseHandle(m_hFile);
        m_hFile = nullptr;
    }
}
CFile* CInternetFile::Duplicate() const {
    return nullptr;
}
void CInternetFile::LockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos;
    (void)dwCount;
}
void CInternetFile::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount) {
    (void)dwPos;
    (void)dwCount;
}
void CInternetFile::Abort() {
    Close();
}
int CInternetFile::QueryOption(DWORD dwOption, void* lpBuffer, DWORD* pdwBufLen) const {
    if (!m_hFile) return FALSE;
    return InternetQueryOptionW(m_hFile, dwOption, lpBuffer, pdwBufLen);
}
int CInternetFile::QueryOption(DWORD dwOption, DWORD& dwValue) const {
    DWORD dwSize = sizeof(dwValue);
    return QueryOption(dwOption, &dwValue, &dwSize);
}
int CInternetFile::SetOption(DWORD dwOption, void* lpBuffer, DWORD dwBufLen, DWORD dwReserved) {
    (void)dwReserved;
    if (!m_hFile) return FALSE;
    return InternetSetOptionW(m_hFile, dwOption, lpBuffer, dwBufLen);
}
wchar_t* CInternetFile::ReadString(wchar_t* pstr, UINT nMax) {
    if (!pstr || nMax == 0) return nullptr;
    std::vector<char> bytes;
    bytes.reserve(nMax);
    for (UINT i = 0; i + 1 < nMax; ++i) {
        char ch = 0;
        if (Read(&ch, 1) != 1) break;
        bytes.push_back(ch);
        if (ch == '\n') break;
    }
    if (bytes.empty()) return nullptr;
    bytes.push_back('\0');
    int converted = MultiByteToWideChar(CP_ACP, 0, bytes.data(), -1, pstr, (int)nMax);
    if (converted <= 0) {
        pstr[0] = L'\0';
        return nullptr;
    }
    return pstr;
}
int CInternetFile::ReadString(CString& rString) {
    wchar_t buf[1024];
    wchar_t* p = ReadString(buf, 1024);
    if (!p) {
        rString.Empty();
        return FALSE;
    }
    rString = p;
    return TRUE;
}
void CInternetFile::WriteString(const wchar_t* pstr) {
    if (!pstr) return;
    int bytesNeeded = WideCharToMultiByte(CP_ACP, 0, pstr, -1, nullptr, 0, nullptr, nullptr);
    if (bytesNeeded <= 1) return;
    std::vector<char> bytes((size_t)bytesNeeded);
    if (!WideCharToMultiByte(CP_ACP, 0, pstr, -1, bytes.data(), bytesNeeded, nullptr, nullptr)) {
        return;
    }
    Write(bytes.data(), (UINT)(bytesNeeded - 1));
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

// === Moved from ManualThunks.cpp ===
// Symbol: ??0CInternetFile@@IEAA@PEAX0PEB_W1_KH@Z
extern "C" void* MS_ABI impl___0CInternetFile__IEAA_PEAX0PEB_W1_KH_Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4, void* p5) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    return nullptr;
}

