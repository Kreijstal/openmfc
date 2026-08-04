// CFileException — OpenMFC implementation.
// Sources: appcore.cpp, filecore.cpp, mfc_exceptions.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"
#include "detail/FilecoreSupport.h"
#include "detail/MfcExceptionsSupport.h"

// Symbol: ?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z(
    const CFileException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) {
    return pThis->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}
// Symbol: ?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ() {
    return &CFileException::classCFileException;
}
int CFileException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
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
    case genericException:
        pszMessage = L"Generic file error";
        break;
    case fileNotFound:
        pszMessage = L"File not found";
        break;
    case badPath:
        pszMessage = L"Invalid path";
        break;
    case tooManyOpenFiles:
        pszMessage = L"Too many open files";
        break;
    case accessDenied:
        pszMessage = L"Access denied";
        break;
    case invalidFile:
        pszMessage = L"Invalid file";
        break;
    case removeCurrentDir:
        pszMessage = L"Cannot remove current directory";
        break;
    case directoryFull:
        pszMessage = L"Directory is full";
        break;
    case badSeek:
        pszMessage = L"Invalid seek operation";
        break;
    case hardIO:
        pszMessage = L"Hardware I/O error";
        break;
    case sharingViolation:
        pszMessage = L"File sharing violation";
        break;
    case lockViolation:
        pszMessage = L"File lock violation";
        break;
    case diskFull:
        pszMessage = L"Disk is full";
        break;
    case endOfFile:
        pszMessage = L"Unexpected end of file";
        break;
    default:
        pszMessage = L"Unknown file error";
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
    } else if (m_lOsError != -1) {
        int written = _snwprintf(lpszError, nMaxError, L"%ls (OS error %ld)", pszMessage, m_lOsError);
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
// Symbol: ?ErrnoToException@CFileException@@SAHH@Z
extern "C" int MS_ABI impl__ErrnoToException_CFileException__SAHH_Z(int nErrno) {
    return FileExceptionFromErrno(nErrno);
}
// Symbol: ?OsErrorToException@CFileException@@SAHJ@Z
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError) {
    return FileExceptionFromOsError(lOsError);
}
// Symbol: ?ThrowErrno@CFileException@@SAXHPEB_W@Z
extern "C" void MS_ABI impl__ThrowErrno_CFileException__SAXHPEB_W_Z(int nErrno, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromErrno(nErrno), static_cast<long>(nErrno), lpszFileName);
}
// Symbol: ?ThrowOsError@CFileException@@SAXJPEB_W@Z
extern "C" void MS_ABI impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long lOsError, const wchar_t* lpszFileName) {
    impl__AfxThrowFileException__YAXHJPEB_W_Z(FileExceptionFromOsError(lOsError), lOsError, lpszFileName);
}
// Symbol: ?GetRuntimeClass@CFileException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileException__UEBAPEAUCRuntimeClass__XZ(const void* pThis) {
    (void)pThis;
    return impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ();
}
// Symbol: ?Dump@CFileException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CFileException__UEBAXXZ(const CFileException* pThis) {
    if (pThis) {
        pThis->CFileException::Dump();
    }
}
// Symbol: ?AssertValid@CFileException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CFileException__UEBAXXZ(const CFileException* pThis) {
    if (pThis) {
        pThis->CFileException::AssertValid();
    }
}
void CFileException::Dump() const {
    wchar_t buffer[512];
    if (!m_strFileName.IsEmpty()) {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException[cause=%d, message=%ls, osError=%ld, file=%ls, autoDelete=%d]\n",
                   m_cause, FileCauseText(m_cause), m_lOsError, static_cast<const wchar_t*>(m_strFileName),
                   m_bAutoDelete);
    } else {
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException[cause=%d, message=%ls, osError=%ld, autoDelete=%d]\n",
                   m_cause, FileCauseText(m_cause), m_lOsError, m_bAutoDelete);
    }
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}
void CFileException::AssertValid() const {
    CException::AssertValid();
    if (m_cause < none || m_cause > endOfFile) {
        wchar_t buffer[96];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CFileException::AssertValid unknown cause=%d\n", m_cause);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}
