#include "InetcoreSupport.h"

IMPLEMENT_DYNAMIC(CInternetSession, CObject)
IMPLEMENT_DYNAMIC(CInternetConnection, CObject)
IMPLEMENT_DYNAMIC(CHttpConnection, CInternetConnection)
IMPLEMENT_DYNAMIC(CFtpConnection, CInternetConnection)
IMPLEMENT_DYNAMIC(CInternetException, CException)
IMPLEMENT_DYNAMIC(CFileFind, CObject)
IMPLEMENT_DYNAMIC(CFtpFileFind, CFileFind)
IMPLEMENT_DYNAMIC(GopherFileFind, CFtpFileFind)
IMPLEMENT_DYNAMIC(CGopherConnection, CInternetConnection)
namespace openmfc { namespace detail { namespace inetcore {
std::unordered_map<HINTERNET, CInternetSession*> g_sessionMap;
thread_local CString g_lastFileName;
CString LocatorToString(const CGopherLocator* pLocator) {
    if (!pLocator || !pLocator->m_lpBuffer || pLocator->m_dwBufferLength == 0) {
        return CString();
    }
    return CString(reinterpret_cast<const wchar_t*>(pLocator->m_lpBuffer));
}
void FillLocator_Inetcore(CGopherLocator* pRet, const CString& locator) {
    if (!pRet) return;
    if (pRet->m_lpBuffer) {
        std::free(pRet->m_lpBuffer);
        pRet->m_lpBuffer = nullptr;
        pRet->m_dwBufferLength = 0;
    }
    new (pRet) CGopherLocator();
    const wchar_t* psz = locator.GetString();
    size_t chars = psz ? wcslen(psz) : 0;
    pRet->m_dwBufferLength = static_cast<DWORD>((chars + 1) * sizeof(wchar_t));
    pRet->m_lpBuffer = std::malloc(pRet->m_dwBufferLength);
    if (pRet->m_lpBuffer) {
        std::memcpy(pRet->m_lpBuffer, psz ? psz : L"", pRet->m_dwBufferLength);
    } else {
        pRet->m_dwBufferLength = 0;
    }
}
CInternetSession* SessionForHandle(HINTERNET hInternet) {
    auto it = g_sessionMap.find(hInternet);
    if (it != g_sessionMap.end()) return it->second;

    HINTERNET current = hInternet;
    for (int i = 0; i < 8; ++i) {
        HINTERNET parent = nullptr;
        DWORD cb = sizeof(parent);
        if (!InternetQueryOptionW(current, INTERNET_OPTION_PARENT_HANDLE, &parent, &cb) || !parent) {
            break;
        }
        it = g_sessionMap.find(parent);
        if (it != g_sessionMap.end()) return it->second;
        if (parent == current) break;
        current = parent;
    }
    return nullptr;
}
void CALLBACK WinInetStatusCallback(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus,
                                   LPVOID lpvStatusInformation, DWORD dwStatusInformationLength) {
    CInternetSession* pSession = SessionForHandle(hInternet);
    if (pSession) {
        pSession->OnStatusCallback(dwContext, dwInternetStatus, lpvStatusInformation,
                                  dwStatusInformationLength);
    }
}
CString MapGopherTypeToString(unsigned long dwType) {
    if (dwType & GOPHER_TYPE_TEXT_FILE) return L"Text File";
    if (dwType & GOPHER_TYPE_DIRECTORY) return L"Directory";
    if (dwType & GOPHER_TYPE_CSO) return L"CSO";
    if (dwType & GOPHER_TYPE_ERROR) return L"Error";
    if (dwType & GOPHER_TYPE_MAC_BINHEX) return L"Mac BinHex";
    if (dwType & GOPHER_TYPE_DOS_ARCHIVE) return L"DOS Archive";
    if (dwType & GOPHER_TYPE_UNIX_UUENCODED) return L"Unix UUEncoded";
    if (dwType & GOPHER_TYPE_INDEX_SERVER) return L"Index Server";
    if (dwType & GOPHER_TYPE_TELNET) return L"Telnet";
    if (dwType & GOPHER_TYPE_BINARY) return L"Binary";
    if (dwType & GOPHER_TYPE_REDUNDANT) return L"Redundant";
    if (dwType & GOPHER_TYPE_TN3270) return L"TN3270";
    if (dwType & GOPHER_TYPE_GIF) return L"GIF";
    if (dwType & GOPHER_TYPE_IMAGE) return L"Image";
    if (dwType & GOPHER_TYPE_BITMAP) return L"Bitmap";
    if (dwType & GOPHER_TYPE_MOVIE) return L"Movie";
    if (dwType & GOPHER_TYPE_SOUND) return L"Sound";
    if (dwType & GOPHER_TYPE_HTML) return L"HTML";
    if (dwType & GOPHER_TYPE_PDF) return L"PDF";
    if (dwType & GOPHER_TYPE_CALENDAR) return L"Calendar";
    return CString();
}
CInternetSession* SessionForStaticHelpers() {
    CInternetSession g_staticHelperSession;
    return &g_staticHelperSession;
}
CString LocatorTypeFromApi(const CGopherLocator& refLocator) {
    const CString locator = LocatorToString(&refLocator);
    if (locator.IsEmpty()) return CString();
    DWORD dwType = 0;
    if (GopherGetLocatorTypeW(locator, &dwType)) {
        return MapGopherTypeToString(dwType);
    }
    return CString();
}
bool BuildGopherLocator(CString& out, const CString& host, INTERNET_PORT port,
                       const CString& display, const CString& selector, DWORD gopherType) {
    CString safeHost = host.IsEmpty() ? CString() : host;
    CString safeDisplay = display;
    CString safeSelector = selector;
    DWORD required = 0;
    std::vector<wchar_t> buffer;
    if (!GopherCreateLocatorW(safeHost, port ? port : INTERNET_DEFAULT_GOPHER_PORT,
                              safeDisplay, safeSelector, gopherType, nullptr, &required)) {
        return false;
    }
    if (required == 0) return false;
    buffer.assign(required, L'\0');
    if (!GopherCreateLocatorW(safeHost, port ? port : INTERNET_DEFAULT_GOPHER_PORT,
                              safeDisplay, safeSelector, gopherType,
                              buffer.data(), &required)) {
        return false;
    }
    out = buffer.data();
    return true;
}
void AppendGopherType(CString& locator, unsigned long gopherType) {
    if (gopherType == 0) return;
    if (!locator.IsEmpty()) locator += L"\t";
    if (gopherType < 128 && iswprint((wchar_t)gopherType)) {
        locator.AppendFormat(L"%lc", static_cast<wchar_t>(gopherType));
    } else {
        locator.AppendFormat(L"%lu", gopherType);
    }
}
CRuntimeClass* AFXAPI gb_CInternetFile() {
    return impl__GetThisClass_CStdioFile__SAPEAUCRuntimeClass__XZ();
}
__attribute__((used)) CRuntimeClass g_classCInternetFile = {
    "CInternetFile", 128, 0xFFFF, nullptr, gb_CInternetFile, nullptr, nullptr
};
__attribute__((used)) CRuntimeClass g_classCInternetException = {
    "CInternetException", sizeof(CInternetException), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};
__attribute__((used)) CRuntimeClass g_classCHttpFile = {
    "CHttpFile", sizeof(CHttpFile), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};
__attribute__((used)) CRuntimeClass g_classCGopherFile = {
    "CGopherFile", sizeof(CGopherFile), 0xFFFF, nullptr, nullptr, nullptr, nullptr
};
void openmfcConstructString(CString* pRet, const CString& value) {
    if (pRet) {
        new (pRet) CString(value);
    }
}
int openmfcFileTimeToCTimeStorage(const FILETIME& fileTime, void* pTime) {
    if (!pTime) return FALSE;
    ULARGE_INTEGER ticks;
    ticks.LowPart = fileTime.dwLowDateTime;
    ticks.HighPart = fileTime.dwHighDateTime;
    if (ticks.QuadPart == 0) {
        std::memset(pTime, 0, sizeof(__int64));
        return TRUE;
    }
    const unsigned __int64 epochDelta = 116444736000000000ULL;
    if (ticks.QuadPart < epochDelta) return FALSE;
    __int64 seconds = (__int64)((ticks.QuadPart - epochDelta) / 10000000ULL);
    std::memcpy(pTime, &seconds, sizeof(seconds));
    return TRUE;
}
} } }  // namespace openmfc::detail::inetcore
