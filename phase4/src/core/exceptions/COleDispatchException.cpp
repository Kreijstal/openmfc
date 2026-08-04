// COleDispatchException — OpenMFC implementation.
// Sources: mfc_exceptions.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfcExceptionsSupport.h"

// Symbol: ??1COleDispatchException@@UEAA@XZ
extern "C" COleDispatchException* MS_ABI impl___1COleDispatchException__UEAA_XZ(
    COleDispatchException* pThis
) {
    if (!pThis) return nullptr;
    {
        std::lock_guard<std::mutex> lock(g_oleDispatchDescriptionMutex);
        g_oleDispatchDescriptions.erase(pThis);
    }
    pThis->~COleDispatchException();
    return pThis;
}
// Symbol: ?GetErrorMessage@COleDispatchException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z(
    const COleDispatchException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!lpszError || nMaxError == 0) return 0;
    if (pnHelpContext) *pnHelpContext = pThis ? pThis->m_dwHelpContext : 0;
    if (!pThis) {
        lpszError[0] = L'\0';
        return 0;
    }

    std::wstring desc;
    {
        std::lock_guard<std::mutex> lock(g_oleDispatchDescriptionMutex);
        auto it = g_oleDispatchDescriptions.find(pThis);
        if (it != g_oleDispatchDescriptions.end()) {
            desc = it->second;
        }
    }
    if (desc.empty() && pThis->m_strDescription) {
        desc = pThis->m_strDescription;
    }
    if (desc.empty()) {
        swprintf(lpszError, nMaxError, L"OLE dispatch exception (code %u)", pThis->m_wCode);
        return 1;
    }
    CopyErrorText(lpszError, nMaxError, desc.c_str());
    return 1;
}
// Symbol: ??0COleDispatchException@@QEAA@PEB_WIG@Z
// Constructor: (const wchar_t* lpszDescription, unsigned int wCode, unsigned short dwHelpContext)
extern "C" COleDispatchException* MS_ABI impl___0COleDispatchException__QEAA_PEB_WIG_Z(
    COleDispatchException* pThis, const wchar_t* lpszDescription,
    unsigned int wCode, unsigned short dwHelpContext
) {
    if (!pThis) return nullptr;
    new (pThis) COleDispatchException();
    pThis->m_wCode = static_cast<WORD>(wCode);
    pThis->m_strDescription = lpszDescription;
    pThis->m_dwHelpContext = static_cast<DWORD>(dwHelpContext);
    return pThis;
}
// Symbol: ?Process@COleDispatchException@@SAXPEAUtagEXCEPINFO@@PEBVCException@@@Z
extern "C" void MS_ABI impl__Process_COleDispatchException__SAXPEAUtagEXCEPINFO__PEBVCException___Z(
    EXCEPINFO* pExcepInfo, const CException* pAnyException
) {
    if (!pExcepInfo || !pAnyException) return;

    if (pAnyException->IsKindOf(&classCOleDispatchException)) {
        const auto* pDE = static_cast<const COleDispatchException*>(pAnyException);
        pExcepInfo->wCode = pDE->m_wCode;
        const wchar_t* appName = (AfxGetApp() && AfxGetApp()->m_pszAppName) ? AfxGetApp()->m_pszAppName : L"";
        pExcepInfo->bstrSource = SysAllocString(appName);
        if (pDE->m_strDescription)
            pExcepInfo->bstrDescription = SysAllocString(pDE->m_strDescription);
        pExcepInfo->dwHelpContext = pDE->m_dwHelpContext;
        return;
    }
    if (pAnyException->IsKindOf(&classCOleException)) {
        const auto* pOE = static_cast<const COleException*>(pAnyException);
        pExcepInfo->wCode = 0;
        pExcepInfo->scode = pOE->m_sc;
        return;
    }
    pExcepInfo->wCode = 0;
    pExcepInfo->scode = E_FAIL;
}
