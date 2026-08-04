// CArchiveException — OpenMFC implementation.
// Sources: appcore.cpp, docview.cpp, manual_small_stub_implementations.cpp, mfc_exceptions.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/DocviewSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfcExceptionsSupport.h"

// Symbol: ?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z(
    const CArchiveException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) {
    return pThis->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}
// Symbol: ?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CArchiveException__SAPEAUCRuntimeClass__XZ() {
    return &CArchiveException::classCArchiveException;
}
CArchiveException::CArchiveException(int cause, const wchar_t* lpszArchiveName)
    : m_cause(cause)
{
    if (lpszArchiveName)
        m_strFileName = lpszArchiveName;
}
int CArchiveException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    if (pnHelpContext != nullptr) {
        *pnHelpContext = 0;
    }

    if (lpszError == nullptr || nMaxError == 0) {
        return 0;
    }

    const wchar_t* pszMessage = nullptr;

    switch (m_cause) {
    case none:
        pszMessage = L"No error";
        break;
    case generic:
        pszMessage = L"Archive error";
        break;
    case readOnly:
        pszMessage = L"Cannot write to read-only archive";
        break;
    case endOfFile:
        pszMessage = L"Unexpected end of file";
        break;
    case writeOnly:
        pszMessage = L"Cannot read from write-only archive";
        break;
    case badIndex:
        pszMessage = L"Invalid object index";
        break;
    case badClass:
        pszMessage = L"Invalid class found in archive";
        break;
    case badSchema:
        pszMessage = L"Schema mismatch in archive";
        break;
    case badFormat:
        pszMessage = L"Bad archive format";
        break;
    default:
        pszMessage = L"Unknown archive error";
        break;
    }

    if (!pszMessage) {
        lpszError[0] = L'\0';
        return 0;
    }

    if (!m_strFileName.IsEmpty()) {
        int written = _snwprintf(
            lpszError, nMaxError, L"%ls: %ls", pszMessage, static_cast<const wchar_t*>(m_strFileName));
        if (written < 0 || static_cast<UINT>(written) >= nMaxError) {
            lpszError[nMaxError - 1] = L'\0';
        }
    } else {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
    }
    lpszError[nMaxError - 1] = L'\0';
    return 1;  // TRUE - message provided
}
// Symbol: ?GetRuntimeClass@CArchiveException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CArchiveException__UEBAPEAUCRuntimeClass__XZ(const CArchiveException*) { return &CArchiveException::classCArchiveException; }
// Implemented in docview.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CArchiveException@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?Dump@CArchiveException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CArchiveException__UEBAXXZ(const CArchiveException* pThis) {
    if (pThis) {
        pThis->CArchiveException::Dump();
    }
}
// Symbol: ?AssertValid@CArchiveException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CArchiveException__UEBAXXZ(const CArchiveException* pThis) {
    if (pThis) {
        pThis->CArchiveException::AssertValid();
    }
}
void CArchiveException::Dump() const {
    wchar_t buffer[512];
    if (!m_strFileName.IsEmpty()) {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException[cause=%d, message=%ls, file=%ls, autoDelete=%d]\n",
                   m_cause, ArchiveCauseText(m_cause), static_cast<const wchar_t*>(m_strFileName), m_bAutoDelete);
    } else {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException[cause=%d, message=%ls, autoDelete=%d]\n",
                   m_cause, ArchiveCauseText(m_cause), m_bAutoDelete);
    }
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}
void CArchiveException::AssertValid() const {
    CException::AssertValid();
    if (m_cause < none || m_cause > badFormat) {
        wchar_t buffer[104];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CArchiveException::AssertValid unknown cause=%d\n", m_cause);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}
