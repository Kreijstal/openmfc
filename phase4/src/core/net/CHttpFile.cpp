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

// === Moved from ManualThunks.cpp ===
//
// LAYOUT NOTE for the thunks below. Retail (mfc140u) keeps CInternetFile::m_hFile at
// this+0x30 -- every retail body below that uses m_hFile loads it from this+0x30 (the
// QueryInfo(CString&) and AddRequestHeaders bodies through a callee-saved copy of this;
// SendRequest(CString&) only indirectly, in its callee at 0x230ac0) -- and CHttpFile's
// m_strObject/m_strVerb at +0x80/+0x88, sizeof(CHttpFile) == 0x90. OpenMFC's
// include/openmfc/afxinet.h does NOT match: measured with this file's compile flags,
// m_hFile is at +0x20, m_strVerb +0x60, m_strObject +0x68, sizeof(CHttpFile) == 0xA0.
// Every CHttpFile this DLL hands out is built by OpenMFC's own C++ constructors
// (CHttpConnection::OpenRequest, CInternetSession::OpenURL), so these thunks read
// m_hFile through the header member -- i.e. at OpenMFC's offset, consistent with every
// other method in this file -- and transcribe only the retail CONTROL FLOW. The layout
// mismatch is reported upstream rather than papered over here.
namespace {
inline HINTERNET HttpFileHandle(const void* pThis) {
    return static_cast<const CHttpFile*>(pThis)->m_hFile;
}
} // namespace

// Sibling thunk (defined in core/net/Thunks.cpp with this exact signature); retail's
// CString overload of SendRequest forwards to it (see below).
extern "C" int MS_ABI impl__SendRequest_CHttpFile__QEAAHPEB_WKPEAXK_Z(
    CHttpFile* pThis, const wchar_t* pstrHeaders, unsigned long dwHeadersLen,
    void* lpOptional, unsigned long dwOptionalLen);

// Symbol: ??0CHttpFile@@IEAA@PEAX0PEB_W11_K@Z
// CHttpFile(HINTERNET hFile, HINTERNET hSession, LPCTSTR pstrObject, LPCTSTR pstrServer,
//           LPCTSTR pstrVerb, DWORD_PTR dwContext)  -- RVA 0x230730 (mfc140u).
// STUB. Retail body: calls the CInternetFile(hFile, hSession, pstrObject, pstrServer,
// dwContext, bReadMode=TRUE) ctor (RVA 0x22e480, mfc140u; it does not store hSession),
// installs the CHttpFile vftable, constructs m_strObject (+0x80) from pstrObject and
// m_strVerb (+0x88) from pstrVerb, then stores hFile into +0x48 (CInternetFile::
// m_hConnection) and returns this. Not implementable against OpenMFC's header: this
// protected ctor is only reachable from a client-derived class, whose storage is sized
// for retail's 0x90-byte CHttpFile, while OpenMFC's C++ CHttpFile is 0xA0 bytes and its
// constructor memsets _httpfile_padding up to +0xA0 -- placement-constructing it would
// write 0x10 bytes past the client's base subobject. OpenMFC's CInternetFile also has no
// m_strServerName / m_bReadMode / m_hConnection to receive pstrServer etc.
// The parameter list below is the one the mangled name describes (the auto-generated
// placeholder had six void*); it returns pThis as the MSVC ctor ABI does, but constructs
// nothing.
extern "C" void* MS_ABI impl___0CHttpFile__IEAA_PEAX0PEB_W11_K_Z(
    void* pThis, void* hFile, void* hSession, const wchar_t* pstrObject,
    const wchar_t* pstrServer, const wchar_t* pstrVerb, unsigned long long dwContext) {
    (void)hFile;
    (void)hSession;
    (void)pstrObject;
    (void)pstrServer;
    (void)pstrVerb;
    (void)dwContext;
    return pThis;
}


// Symbol: ?AddRequestHeaders@CHttpFile@@QEAAHPEB_WKH@Z
// BOOL AddRequestHeaders(LPCTSTR pstrHeaders, DWORD dwFlags, int dwHeadersLen).
// Transcribed from RVA 0x230a50 (mfc140u): when dwHeadersLen == -1 it is replaced by
// wcslen(pstrHeaders) (IAT slot 0x1802c7748 (mfc140u) = wcslen), or 0 when pstrHeaders is NULL;
// then tail-jumps to ::HttpAddRequestHeadersW(m_hFile, pstrHeaders, len, dwFlags)
// (delay-load slot 0x1803e9210 (mfc140u), resolved with dlyu.py). No NULL check on m_hFile.
extern "C" int MS_ABI impl__AddRequestHeaders_CHttpFile__QEAAHPEB_WKH_Z(
    void* pThis, const wchar_t* pstrHeaders, unsigned long dwFlags, int dwHeadersLen) {
    DWORD dwLen = (DWORD)dwHeadersLen;
    if (dwHeadersLen == -1)
        dwLen = pstrHeaders ? (DWORD)wcslen(pstrHeaders) : 0;
    return ::HttpAddRequestHeadersW(HttpFileHandle(pThis), pstrHeaders, dwLen, dwFlags);
}


// Symbol: ?QueryInfo@CHttpFile@@QEBAHKAEAKPEAK@Z
// BOOL QueryInfo(DWORD dwInfoLevel, DWORD& dwResult, LPDWORD lpdwIndex) const.
// Transcribed from RVA 0x230c20 (mfc140u; ordinal-resolved, not in the symbol map):
// cb = 4; return ::HttpQueryInfoW(m_hFile, dwInfoLevel | HTTP_QUERY_FLAG_NUMBER
// (`bts $0x1d` = 0x20000000), &dwResult, &cb, lpdwIndex) (delay-load slot 0x1803e91f0 (mfc140u)).
extern "C" int MS_ABI impl__QueryInfo_CHttpFile__QEBAHKAEAKPEAK_Z(
    const void* pThis, unsigned long dwInfoLevel, unsigned long* pdwResult, unsigned long* lpdwIndex) {
    DWORD cb = sizeof(DWORD);
    return ::HttpQueryInfoW(HttpFileHandle(pThis), dwInfoLevel | HTTP_QUERY_FLAG_NUMBER,
                            pdwResult, &cb, lpdwIndex);
}


// Symbol: ?QueryInfo@CHttpFile@@QEBAHKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAK@Z
// BOOL QueryInfo(DWORD dwInfoLevel, CString& str, LPDWORD lpdwIndex) const.
// Transcribed from RVA 0x230d10 (mfc140u; ordinal-resolved, not in the symbol map):
//   cb = 0; str.Empty() (call to CSimpleStringT::Empty, RVA 0x33b0 mfc140u);
//   if (::HttpQueryInfoW(m_hFile, dwInfoLevel, NULL, &cb, NULL)) return TRUE;
//      -- note the probe passes a NULL index, not lpdwIndex
//   buf = str.GetBufferSetLength(cb / 2);           (cb is bytes; `shr $1`)
//   bRet = ::HttpQueryInfoW(m_hFile, dwInfoLevel, buf, &cb, lpdwIndex);
//   str.ReleaseBufferSetLength(bRet ? cb / 2 : 0);
//   return bRet;
// There is no GetLastError() test: the second call is made whatever the first failed
// with. Deviation: retail raises E_INVALIDARG (AtlThrow) if the reported length exceeds
// the buffer; this body clamps to the buffer size instead (OpenMFC's inline CString has
// no AtlThrow path) -- HttpQueryInfoW cannot report more than it was given on success.
extern "C" int MS_ABI impl__QueryInfo_CHttpFile__QEBAHKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAK_Z(
    const void* pThis, unsigned long dwInfoLevel, CString* pStr, unsigned long* lpdwIndex) {
    HINTERNET hFile = HttpFileHandle(pThis);
    DWORD cb = 0;
    pStr->Empty();
    if (::HttpQueryInfoW(hFile, dwInfoLevel, nullptr, &cb, nullptr))
        return TRUE;
    const int nCap = (int)(cb >> 1);
    if (nCap == 0) {
        // Retail hands the (empty) string's own buffer to the second call with cb == 0.
        // OpenMFC's inline GetBuffer(0)/ReleaseBuffer(0) on the shared nil string would
        // overwrite the nil sentinel's locked refcount, so use a local empty buffer
        // instead. str stays empty here in every case; retail differs only if this
        // call succeeds and writes back a cb >= 2 (it would then set that length, or
        // AtlThrow), a case not reproduced here.
        wchar_t chNil = L'\0';
        return ::HttpQueryInfoW(hFile, dwInfoLevel, &chNil, &cb, lpdwIndex);
    }
    wchar_t* pBuf = pStr->GetBuffer(nCap);
    pBuf[nCap] = L'\0';
    BOOL bRet = ::HttpQueryInfoW(hFile, dwInfoLevel, pBuf, &cb, lpdwIndex);
    int nLen = 0;
    if (bRet) {
        nLen = (int)(cb >> 1);
        if (nLen > nCap) nLen = nCap;   // retail: AtlThrow(E_INVALIDARG) -- see above
    }
    pStr->ReleaseBuffer(nLen);
    return bRet;
}


// Symbol: ?QueryInfo@CHttpFile@@QEBAHKPEAU_SYSTEMTIME@@PEAK@Z
// BOOL QueryInfo(DWORD dwInfoLevel, SYSTEMTIME* pSysTime, LPDWORD lpdwIndex) const.
// Transcribed from RVA 0x230c50 (mfc140u; ordinal-resolved, not in the symbol map):
// cb = 0x10 (sizeof(SYSTEMTIME)); return ::HttpQueryInfoW(m_hFile, dwInfoLevel |
// HTTP_QUERY_FLAG_SYSTEMTIME (`bts $0x1e` = 0x40000000), pSysTime, &cb, lpdwIndex).
// Retail neither NULL-checks nor clears pSysTime.
extern "C" int MS_ABI impl__QueryInfo_CHttpFile__QEBAHKPEAU_SYSTEMTIME__PEAK_Z(
    const void* pThis, unsigned long dwInfoLevel, SYSTEMTIME* pSysTime, unsigned long* lpdwIndex) {
    DWORD cb = sizeof(SYSTEMTIME);
    return ::HttpQueryInfoW(HttpFileHandle(pThis), dwInfoLevel | HTTP_QUERY_FLAG_SYSTEMTIME,
                            pSysTime, &cb, lpdwIndex);
}


// Symbol: ?QueryInfo@CHttpFile@@QEBAHKPEAXPEAK1@Z
// BOOL QueryInfo(DWORD dwInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength,
//                LPDWORD lpdwIndex) const.
// Transcribed from RVA 0x230c10 (mfc140u; ordinal-resolved, not in the symbol map): loads
// m_hFile into RCX and tail-jumps to ::HttpQueryInfoW with the remaining arguments
// untouched (delay-load slot 0x1803e91f0 (mfc140u)).
extern "C" int MS_ABI impl__QueryInfo_CHttpFile__QEBAHKPEAXPEAK1_Z(
    const void* pThis, unsigned long dwInfoLevel, void* lpvBuffer,
    unsigned long* lpdwBufferLength, unsigned long* lpdwIndex) {
    return ::HttpQueryInfoW(HttpFileHandle(pThis), dwInfoLevel, lpvBuffer,
                            lpdwBufferLength, lpdwIndex);
}


// Symbol: ?SendRequest@CHttpFile@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAXK@Z
// BOOL SendRequest(CString& strHeaders, LPVOID lpOptional, DWORD dwOptionalLen).
// Transcribed from RVA 0x230bf0 (mfc140u; ordinal-resolved, not in the symbol map): calls
// SendRequest(LPCTSTR, DWORD, LPVOID, DWORD) (RVA 0x230ac0 mfc140u) with
// (strHeaders buffer, strHeaders.GetLength(), lpOptional, dwOptionalLen). Here that
// sibling is reached through its impl__ thunk. DEVIATION inherited from that sibling:
// retail's 0x230ac0 calls AfxThrowInternetException(m_dwContext, 0) (RVA 0x231980,
// mfc140u) when ::HttpSendRequestW fails, so retail never returns FALSE here; OpenMFC's
// sibling (Thunks.cpp -> CHttpFile::SendRequest in this file) returns FALSE instead.
extern "C" int MS_ABI impl__SendRequest_CHttpFile__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAXK_Z(
    void* pThis, CString* pStrHeaders, void* lpOptional, unsigned long dwOptionalLen) {
    return impl__SendRequest_CHttpFile__QEAAHPEB_WKPEAXK_Z(
        static_cast<CHttpFile*>(pThis), pStrHeaders->GetString(),
        (unsigned long)pStrHeaders->GetLength(), lpOptional, dwOptionalLen);
}
