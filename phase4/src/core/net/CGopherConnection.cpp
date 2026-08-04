// CGopherConnection — OpenMFC implementation.
// Sources: inetcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/InetcoreSupport.h"

// Symbol: ?GetAttribute@CGopherConnection@@QEAAHAEAVCGopherLocator@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAV34@@Z
extern "C" int MS_ABI impl__GetAttribute_CGopherConnection__QEAAHAEAVCGopherLocator__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAV34__Z(
    CGopherConnection* pThis, CGopherLocator* pLocator, CString* pAttributeName, CString* pValue) {
    (void)pThis;
    if (!pLocator || !pValue) return FALSE;
    const wchar_t* name = pAttributeName ? (const wchar_t*)*pAttributeName : nullptr;
    if (name && (_wcsicmp(name, L"Length") == 0 || _wcsicmp(name, L"Size") == 0)) {
        *pValue = L"0";
    } else if (name && _wcsicmp(name, L"Type") == 0) {
        *pValue = L"Unknown";
    } else {
        pValue->Empty();
    }
    return TRUE;
}
// Symbol: ?GetRuntimeClass@CGopherConnection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGopherConnection__UEBAPEAUCRuntimeClass__XZ(const CGopherConnection* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CGopherConnection::GetThisClass();
}
// Symbol: ?GetThisClass@CGopherConnection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGopherConnection__SAPEAUCRuntimeClass__XZ() {
    return CGopherConnection::GetThisClass();
}
// Symbol: ??0CGopherConnection@@QEAA@PEAVCInternetSession@@PEAXPEB_W_K@Z
extern "C" void* MS_ABI impl___0CGopherConnection__QEAA_PEAVCInternetSession__PEAXPEB_W_K_Z(
    void* pThis, CInternetSession* p0, void* p1, const wchar_t* p2, unsigned __int64 p3) {
    return new(pThis) CGopherConnection(p0, p1, p2, p3);
}
// Symbol: ??0CGopherConnection@@QEAA@PEAVCInternetSession@@PEB_W11_KG@Z
extern "C" void* MS_ABI impl___0CGopherConnection__QEAA_PEAVCInternetSession__PEB_W11_KG_Z(
    void* pThis, CInternetSession* p0, const wchar_t* p1, const wchar_t* p2,
    const wchar_t* p3, unsigned __int64 p4, unsigned short p5) {
    return new(pThis) CGopherConnection(p0, p1, p2, p3, p4, p5);
}
CGopherConnection::CGopherConnection(CInternetSession* pSession, HINTERNET hConnected,
                                      const wchar_t* pstrServer, DWORD_PTR dwContext)
    : CInternetConnection(pSession, pstrServer, INTERNET_DEFAULT_GOPHER_PORT, dwContext) {
    m_hConnection = hConnected;
    if (!m_hConnection && m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            INTERNET_DEFAULT_GOPHER_PORT,
            nullptr,
            nullptr,
            INTERNET_SERVICE_GOPHER,
            0,
            dwContext
        );
    }
    memset(_gopherconn_padding, 0, sizeof(_gopherconn_padding));
}
CGopherConnection::CGopherConnection(CInternetSession* pSession, const wchar_t* pstrServer,
                                      const wchar_t* pstrUserName, const wchar_t* pstrPassword,
                                      DWORD_PTR dwContext, INTERNET_PORT nPort)
    : CInternetConnection(pSession, pstrServer, nPort, dwContext) {
    if (!m_hConnection && m_pSession && m_pSession->GetHandle()) {
        m_hConnection = InternetConnectW(
            m_pSession->GetHandle(),
            pstrServer,
            nPort,
            pstrUserName,
            pstrPassword,
            INTERNET_SERVICE_GOPHER,
            0,
            dwContext
        );
    }
    memset(_gopherconn_padding, 0, sizeof(_gopherconn_padding));
}
CGopherConnection::~CGopherConnection() {}
CGopherFile* CGopherConnection::OpenFile(GOPHER_FIND_DATAW* pFindData,
                                          const wchar_t* pstrLocator,
                                          DWORD_PTR dwContext) {
    const wchar_t* locator = pstrLocator;
    if (!locator && pFindData && pFindData->Locator[0]) locator = pFindData->Locator;
    if (!m_hConnection || !locator) return nullptr;

    HINTERNET hFile = GopherOpenFileW(m_hConnection, locator, nullptr, 0, dwContext);
    if (!hFile) return nullptr;

    CGopherLocator locatorValue;
    FillLocator_Inetcore(&locatorValue, CString(locator));
    return new CGopherFile(hFile, locatorValue, this);
}
CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 CGopherLocator* pLocator,
                                                 unsigned long dwContext) {
    (void)dwContext;
    CGopherLocator loc;
    CString display = pstrDisplayString ? pstrDisplayString : L"";
    CString selector = LocatorToString(pLocator);
    DWORD gopherType = GOPHER_TYPE_UNKNOWN;
    if (pLocator && !selector.IsEmpty()) {
        GopherGetLocatorTypeW(selector, &gopherType);
    }

    CString built;
    if (BuildGopherLocator(built, m_strServerName, m_nPort, display, selector, gopherType)) {
        FillLocator_Inetcore(&loc, built);
        return loc;
    }

    CString locator = display;
    if (!locator.IsEmpty() && !selector.IsEmpty()) locator += L"\t";
    locator += selector;
    FillLocator_Inetcore(&loc, locator);
    return loc;
}
CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrLocator) {
    CGopherLocator loc;
    if (!pstrLocator) return loc;
    FillLocator_Inetcore(&loc, pstrLocator ? pstrLocator : L"");
    return loc;
}
CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 CGopherLocator* pLocator1,
                                                 CGopherLocator* pLocator2,
                                                 unsigned long dwContext,
                                                 unsigned short nGopherType) {
    (void)dwContext;
    CGopherLocator loc;
    CString display = pstrDisplayString ? pstrDisplayString : L"";
    CString left = LocatorToString(pLocator1);
    CString right = LocatorToString(pLocator2);

    CString selector;
    if (!left.IsEmpty()) {
        if (!selector.IsEmpty()) selector += L"\t";
        selector += left;
    }
    if (!right.IsEmpty()) {
        if (!selector.IsEmpty()) selector += L"\t";
        selector += right;
    }

    CString built;
    if (BuildGopherLocator(built, CString(), INTERNET_INVALID_PORT_NUMBER, display, selector, nGopherType)) {
        FillLocator_Inetcore(&loc, built);
        return loc;
    }

    CString locator = display;
    if (!locator.IsEmpty() && !selector.IsEmpty()) locator += L"\t";
    locator += selector;
    AppendGopherType(locator, nGopherType);
    FillLocator_Inetcore(&loc, locator);
    return loc;
}
CGopherLocator CGopherConnection::CreateLocator(const wchar_t* pstrDisplayString,
                                                 const wchar_t* pstrSelectorString,
                                                 DWORD dwGopherType) {
    CGopherLocator loc;
    CString display = pstrDisplayString ? pstrDisplayString : L"";
    CString selector = pstrSelectorString ? pstrSelectorString : L"";

    CString built;
    if (BuildGopherLocator(built, m_strServerName, m_nPort, display, selector, dwGopherType)) {
        FillLocator_Inetcore(&loc, built);
        return loc;
    }

    CString locator = display;
    if (!locator.IsEmpty() && !selector.IsEmpty()) locator += L"\t";
    locator += selector;
    AppendGopherType(locator, dwGopherType);
    FillLocator_Inetcore(&loc, locator);
    return loc;
}
CString CGopherConnection::GetLocatorType(const CGopherLocator& refLocator) {
    CString type = LocatorTypeFromApi(refLocator);
    if (!type.IsEmpty()) return type;
    CString locator = LocatorToString(&refLocator);
    if (!locator.IsEmpty()) return locator.Left(1);
    return CString();
}
CFtpFileFind* CGopherConnection::CreateFileFind(CGopherLocator* pLocator) {
    auto* pFind = new GopherFileFind(reinterpret_cast<CFtpConnection*>(this), m_dwContext);
    if (pFind && pLocator) pFind->m_strRoot = LocatorToString(pLocator);
    return pFind;
}
