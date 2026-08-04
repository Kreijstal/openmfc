// CHttpFile — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ??0CHttpFile@@IEAA@PEAXPEB_W1PEAVCHttpConnection@@@Z
extern "C" void* MS_ABI impl___0CHttpFile__IEAA_PEAXPEB_W1PEAVCHttpConnection___Z(
    void* pThis, void* hFile, const wchar_t* pstrVerb, const wchar_t* pstrObjectName, CHttpConnection* pConnection) {
    return new (pThis) CHttpFile((HINTERNET)hFile, pstrVerb, pstrObjectName, pConnection);
}
// Symbol: ?AddRequestHeaders@CHttpFile@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@K@Z
extern "C" int MS_ABI impl__AddRequestHeaders_CHttpFile__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__K_Z(
    CHttpFile* pThis, CString& strHeaders, unsigned long dwFlags) {
    return pThis->AddRequestHeaders(strHeaders, dwFlags);
}
// Symbol: ?ErrorDlg@CHttpFile@@QEAAKPEAVCWnd@@KKPEAPEAX@Z
extern "C" unsigned long MS_ABI impl__ErrorDlg_CHttpFile__QEAAKPEAVCWnd__KKPEAPEAX_Z(
    CHttpFile* pThis, CWnd* pParentWnd, unsigned long dwError, unsigned long dwFlags, void** ppvData) {
    return pThis->ErrorDlg(pParentWnd, dwError, dwFlags, ppvData);
}
// Symbol: ?GetObjectW@CHttpFile@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetObjectW_CHttpFile__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHttpFile* pThis) {
    new (pRet) CString(pThis->GetObjectW());
}
// Symbol: ?GetVerb@CHttpFile@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetVerb_CHttpFile__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHttpFile* pThis) {
    new (pRet) CString(pThis->GetVerb());
}
// Symbol: ?GetFileURL@CHttpFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetFileURL_CHttpFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* pRet, const CHttpFile* pThis) {
    new (pRet) CString(pThis->GetFileURL());
}
// Symbol: ?GetRuntimeClass@CHttpFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHttpFile__UEBAPEAUCRuntimeClass__XZ(
    const CHttpFile* pThis) {
    (void)pThis;
    return &g_classCHttpFile;
}
// Symbol: ?GetThisClass@CHttpFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHttpFile__SAPEAUCRuntimeClass__XZ() {
    return &g_classCHttpFile;
}
CHttpFile::CHttpFile(HINTERNET hFile, const wchar_t* pstrVerb, const wchar_t* pstrObjectName,
                     CHttpConnection* pConnection)
    : CHttpFile(hFile, pConnection ? pConnection->GetHandle() : nullptr,
                pstrVerb, pstrObjectName, pConnection) {
}
CHttpFile::CHttpFile(HINTERNET hFile, HINTERNET hConnect,
                     const wchar_t* pstrVerb, const wchar_t* pstrObjectName,
                     CHttpConnection* pConnection)
    : CInternetFile(), m_bHttps(FALSE), m_pConnection(pConnection),
      m_strVerb(pstrVerb ? pstrVerb : L""),
      m_strObject(pstrObjectName ? pstrObjectName : L""),
      m_hConnect(hConnect)
{
    m_hFile = hFile;
    memset(_httpfile_padding, 0, sizeof(_httpfile_padding));
}
CHttpFile::~CHttpFile() {
}
int CHttpFile::AddRequestHeaders(const wchar_t* pstrHeaders, DWORD dwFlags) {
    if (!m_hFile) return FALSE;
    return HttpAddRequestHeadersW(m_hFile, pstrHeaders, (DWORD)-1, dwFlags);
}
int CHttpFile::AddRequestHeaders(CString& str, DWORD dwFlags) {
    return AddRequestHeaders((const wchar_t*)str, dwFlags);
}
int CHttpFile::SendRequest(const wchar_t* pstrHeaders, DWORD dwHeadersLen,
                            void* lpOptional, DWORD dwOptionalLen) {
    if (!m_hFile) return FALSE;
    return HttpSendRequestW(m_hFile, pstrHeaders, dwHeadersLen, lpOptional, dwOptionalLen);
}
int CHttpFile::SendRequest(CString& strHeaders, DWORD dwHeadersLen,
                            void* lpOptional, DWORD dwOptionalLen) {
    return SendRequest((const wchar_t*)strHeaders, dwHeadersLen, lpOptional, dwOptionalLen);
}
int CHttpFile::SendRequestEx(DWORD dwTotalLen, DWORD dwFlags, DWORD_PTR dwContext) {
    if (!m_hFile) return FALSE;
    INTERNET_BUFFERS buffer = {};
    buffer.dwStructSize = sizeof(buffer);
    buffer.dwBufferTotal = dwTotalLen;
    return HttpSendRequestExW(m_hFile, &buffer, nullptr, dwFlags, dwContext);
}
int CHttpFile::EndRequest(DWORD dwFlags, LPINTERNET_BUFFERS lpBuffIn, DWORD_PTR dwContext) {
    if (!m_hFile) return FALSE;
    return HttpEndRequestW(m_hFile, lpBuffIn, dwFlags, dwContext);
}
int CHttpFile::QueryInfo(DWORD dwInfoLevel, void* lpBuffer, DWORD* pdwBufferLength,
                          DWORD* pdwIndex) {
    if (!m_hFile) return FALSE;
    return HttpQueryInfoW(m_hFile, dwInfoLevel, lpBuffer, pdwBufferLength, pdwIndex);
}
int CHttpFile::QueryInfoStatusCode(DWORD& dwStatusCode) const {
    if (!m_hFile) return FALSE;
    DWORD dwSize = sizeof(dwStatusCode);
    return HttpQueryInfoW(m_hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                           &dwStatusCode, &dwSize, nullptr);
}
CString CHttpFile::GetVerb() const {
    return m_strVerb;
}
CString CHttpFile::GetObject() const {
    return m_strObject;
}
CString CHttpFile::GetObjectW() const {
    return m_strObject;
}
CString CHttpFile::GetFileURL() const {
    CString url = L"http";
    if (m_bHttps) url += L"s";
    url += L"://";
    if (m_pConnection) {
        url += m_pConnection->GetServerName();
    }
    url += m_strObject;
    return url;
}
void CHttpFile::Close() {
    if (m_hFile) {
        InternetCloseHandle(m_hFile);
        m_hFile = nullptr;
    }
}
DWORD CHttpFile::ErrorDlg(CWnd* pParentWnd, DWORD dwError, DWORD dwFlags, void** ppvData) {
    HWND hWnd = nullptr;
    if (pParentWnd) {
        hWnd = pParentWnd->GetSafeHwnd();
    }
    return InternetErrorDlg(hWnd, m_hFile, dwError, dwFlags,
                            ppvData ? (LPVOID*)ppvData : nullptr);
}
int CHttpFile::QueryInfo(DWORD dwInfoLevel, CString& str, DWORD* pdwIndex) {
    if (!m_hFile) {
        str.Empty();
        return FALSE;
    }
    DWORD chars = 0;
    if (::HttpQueryInfoW(m_hFile, dwInfoLevel, nullptr, &chars, pdwIndex)) {
        str.Empty();
        return TRUE;
    }
    if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER || chars == 0) {
        str.Empty();
        return FALSE;
    }
    std::vector<wchar_t> buffer((size_t)chars + 1, L'\0');
    if (!::HttpQueryInfoW(m_hFile, dwInfoLevel, buffer.data(), &chars, pdwIndex)) {
        str.Empty();
        return FALSE;
    }
    str = buffer.data();
    return TRUE;
}
int CHttpFile::QueryInfo(DWORD dwInfoLevel, SYSTEMTIME* pSysTime, DWORD* pdwIndex) {
    if (!m_hFile || !pSysTime) return FALSE;
    DWORD cb = sizeof(SYSTEMTIME);
    memset(pSysTime, 0, sizeof(SYSTEMTIME));
    return ::HttpQueryInfoW(m_hFile, dwInfoLevel | HTTP_QUERY_FLAG_SYSTEMTIME, pSysTime, &cb, pdwIndex);
}
int CHttpFile::QueryInfoStatusCode(DWORD_PTR& dwStatusCode) const {
    dwStatusCode = 0;
    if (!m_hFile) return 0;
    DWORD dwCode = 0;
    DWORD dwSize = sizeof(DWORD);
    if (::HttpQueryInfoW(m_hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                          &dwCode, &dwSize, nullptr)) {
        dwStatusCode = dwCode;
        return 1;
    }
    return 0;
}
int CHttpFile::SendRequestEx(LPINTERNET_BUFFERS lpBuffIn, LPINTERNET_BUFFERS lpBuffOut,
                              DWORD dwFlags, DWORD_PTR dwContext) {
    if (!m_hFile) return FALSE;
    return ::HttpSendRequestExW(m_hFile, lpBuffIn, lpBuffOut, dwFlags, dwContext);
}
