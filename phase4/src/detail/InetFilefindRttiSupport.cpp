#include "InetFilefindRttiSupport.h"

namespace openmfc { namespace detail { namespace inetfilefindrtti {
std::unordered_map<const void*, GopherFindState>& GopherStates() {
    thread_local std::unordered_map<const void*, GopherFindState> states;
    return states;
}
bool HasFileTime(const FILETIME& ft) {
    return ft.dwLowDateTime != 0 || ft.dwHighDateTime != 0;
}
long long FileTimeToCTimeValue(const FILETIME& ft) {
    ULARGE_INTEGER u;
    u.LowPart = ft.dwLowDateTime;
    u.HighPart = ft.dwHighDateTime;
    return static_cast<long long>(u.QuadPart / 10000000ULL) - 11644473600LL;
}
const wchar_t* LocatorString(const CGopherLocator* pLocator) {
    if (!pLocator || !pLocator->m_lpBuffer || pLocator->m_dwBufferLength == 0) {
        return L"";
    }
    return reinterpret_cast<const wchar_t*>(pLocator->m_lpBuffer);
}
CString FinderName(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end() && it->second.data.DisplayString[0]) {
        return CString(it->second.data.DisplayString);
    }
    if (!pThis->m_strFileName.IsEmpty()) return pThis->m_strFileName;
    return CString(pThis->m_findFileData.cFileName);
}
CString FinderLocator(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) {
        if (it->second.data.Locator[0]) return CString(it->second.data.Locator);
        return it->second.locator;
    }
    return pThis->m_strRoot;
}
FILETIME FinderWriteTime(const CFtpFileFind* pThis) {
    FILETIME ft = {};
    if (!pThis) return ft;
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) return it->second.data.LastModificationTime;
    return pThis->m_findFileData.ftLastWriteTime;
}
ULONGLONG FinderLength(const CFtpFileFind* pThis) {
    if (!pThis) return 0;
    auto it = GopherStates().find(pThis);
    if (it != GopherStates().end()) {
        return (static_cast<ULONGLONG>(it->second.data.SizeHigh) << 32) | it->second.data.SizeLow;
    }
    return (static_cast<ULONGLONG>(pThis->m_findFileData.nFileSizeHigh) << 32) |
           pThis->m_findFileData.nFileSizeLow;
}
CString FinderUrl(const CFtpFileFind* pThis) {
    if (!pThis) return CString();
    CString locator = FinderLocator(pThis);
    CInternetConnection* pConnection =
        reinterpret_cast<CInternetConnection*>(pThis->m_pFtpConnection);
    if (pConnection && !pConnection->GetServerName().IsEmpty()) {
        return CString(L"gopher://") + pConnection->GetServerName() + L"/" + locator;
    }
    return locator;
}
CString FormatGopherAttribute(const GOPHER_ATTRIBUTE_TYPE& attr) {
    CString value;
    switch (attr.AttributeId) {
    case GOPHER_ATTRIBUTE_ID_ADMIN:
        if (attr.AttributeType.Admin.Comment) {
            value = attr.AttributeType.Admin.Comment;
        }
        if (attr.AttributeType.Admin.EmailAddress) {
            if (!value.IsEmpty()) value += L" ";
            value += attr.AttributeType.Admin.EmailAddress;
        }
        break;
    case GOPHER_ATTRIBUTE_ID_MOD_DATE: {
        const FILETIME& ft = attr.AttributeType.ModDate.DateAndTime;
        value.Format(L"%08lx:%08lx",
                     static_cast<unsigned long>(ft.dwHighDateTime),
                     static_cast<unsigned long>(ft.dwLowDateTime));
        break;
    }
    case GOPHER_ATTRIBUTE_ID_TTL:
        value.Format(L"%lu", static_cast<unsigned long>(attr.AttributeType.Ttl.Ttl));
        break;
    case GOPHER_ATTRIBUTE_ID_SCORE:
        value.Format(L"%d", attr.AttributeType.Score.Score);
        break;
    case GOPHER_ATTRIBUTE_ID_RANGE:
        value.Format(L"%d-%d",
                     attr.AttributeType.ScoreRange.LowerBound,
                     attr.AttributeType.ScoreRange.UpperBound);
        break;
    case GOPHER_ATTRIBUTE_ID_SITE:
        if (attr.AttributeType.Site.Site) value = attr.AttributeType.Site.Site;
        break;
    case GOPHER_ATTRIBUTE_ID_ORG:
        if (attr.AttributeType.Organization.Organization) {
            value = attr.AttributeType.Organization.Organization;
        }
        break;
    case GOPHER_ATTRIBUTE_ID_LOCATION:
        if (attr.AttributeType.Location.Location) value = attr.AttributeType.Location.Location;
        break;
    case GOPHER_ATTRIBUTE_ID_GEOG:
        value.Format(L"%d %d %d, %d %d %d",
                     attr.AttributeType.GeographicalLocation.DegreesNorth,
                     attr.AttributeType.GeographicalLocation.MinutesNorth,
                     attr.AttributeType.GeographicalLocation.SecondsNorth,
                     attr.AttributeType.GeographicalLocation.DegreesEast,
                     attr.AttributeType.GeographicalLocation.MinutesEast,
                     attr.AttributeType.GeographicalLocation.SecondsEast);
        break;
    case GOPHER_ATTRIBUTE_ID_TIMEZONE:
        value.Format(L"%d", attr.AttributeType.TimeZone.Zone);
        break;
    case GOPHER_ATTRIBUTE_ID_PROVIDER:
        if (attr.AttributeType.Provider.Provider) value = attr.AttributeType.Provider.Provider;
        break;
    case GOPHER_ATTRIBUTE_ID_VERSION:
        if (attr.AttributeType.Version.Version) value = attr.AttributeType.Version.Version;
        break;
    case GOPHER_ATTRIBUTE_ID_ABSTRACT:
        if (attr.AttributeType.Abstract.ShortAbstract) {
            value = attr.AttributeType.Abstract.ShortAbstract;
        }
        if (attr.AttributeType.Abstract.AbstractFile) {
            if (!value.IsEmpty()) value += L" ";
            value += attr.AttributeType.Abstract.AbstractFile;
        }
        break;
    case GOPHER_ATTRIBUTE_ID_VIEW:
        if (attr.AttributeType.View.ContentType) value = attr.AttributeType.View.ContentType;
        if (attr.AttributeType.View.Language) {
            if (!value.IsEmpty()) value += L"; ";
            value += attr.AttributeType.View.Language;
        }
        if (attr.AttributeType.View.Size) {
            value.AppendFormat(L" (%lu)", static_cast<unsigned long>(attr.AttributeType.View.Size));
        }
        break;
    case GOPHER_ATTRIBUTE_ID_TREEWALK:
        value = attr.AttributeType.Veronica.TreeWalk ? L"TRUE" : L"FALSE";
        break;
    case GOPHER_ATTRIBUTE_ID_UNKNOWN:
    default:
        if (attr.AttributeType.Unknown.Text) value = attr.AttributeType.Unknown.Text;
        break;
    }
    return value;
}
void FillLocator_InetFilefindRtti(CGopherLocator* pRet, const CString& locator) {
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
void CloseGopherFinder(CFtpFileFind* pThis) {
    if (!pThis) return;
    if (pThis->m_hFindHandle) {
        ::InternetCloseHandle(pThis->m_hFindHandle);
        pThis->m_hFindHandle = nullptr;
    }
    GopherStates().erase(pThis);
}
int FindGopher(CFtpFileFind* pThis, const wchar_t* locator,
               const wchar_t* search, DWORD flags) {
    if (!pThis) return 0;
    CloseGopherFinder(pThis);
    CInternetConnection* pConnection =
        reinterpret_cast<CInternetConnection*>(pThis->m_pFtpConnection);
    if (!pConnection || !pConnection->m_hConnection) return 0;

    GopherFindState state;
    std::memset(&state.data, 0, sizeof(state.data));
    state.locator = locator ? locator : L"";
    HINTERNET hFind = ::GopherFindFirstFileW(pConnection->m_hConnection,
                                             locator,
                                             search,
                                             &state.data,
                                             flags,
                                             pThis->m_dwContext);
    if (!hFind) return 0;

    pThis->m_hFindHandle = hFind;
    std::memset(&pThis->m_findFileData, 0, sizeof(pThis->m_findFileData));
    wcsncpy(pThis->m_findFileData.cFileName, state.data.DisplayString,
            MAX_PATH - 1);
    pThis->m_findFileData.cFileName[MAX_PATH - 1] = L'\0';
    pThis->m_findFileData.nFileSizeLow = state.data.SizeLow;
    pThis->m_findFileData.nFileSizeHigh = state.data.SizeHigh;
    pThis->m_findFileData.ftLastWriteTime = state.data.LastModificationTime;
    pThis->m_strFileName = state.data.DisplayString;
    GopherStates()[pThis] = state;
    return 1;
}
} } }  // namespace openmfc::detail::inetfilefindrtti
