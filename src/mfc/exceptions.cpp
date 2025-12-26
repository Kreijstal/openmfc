// Define OPENMFC_APPCORE_IMPL since we link with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <new>

// Exception throwing helpers

void AFXAPI AfxThrowMemoryException() {
    throw new CMemoryException();
}

void AFXAPI AfxThrowFileException(int cause, LONG lOsError, LPCWSTR lpszFileName) {
    CFileException* pEx = new CFileException(cause, lOsError);
    if (lpszFileName != nullptr) {
        pEx->m_strFileName = lpszFileName;
    }
    throw pEx;
}

void AFXAPI AfxThrowInvalidArgException() {
    // For now, use CException as base if specific class not implemented
    // TODO: Implement CInvalidArgException
    throw new CException(TRUE); 
}

void AFXAPI AfxThrowNotSupportedException() {
    // TODO: Implement CNotSupportedException
    throw new CException(TRUE);
}

void AFXAPI AfxThrowResourceException() {
    // TODO: Implement CResourceException
    throw new CException(TRUE);
}

void AFXAPI AfxThrowUserException() {
    // TODO: Implement CUserException
    throw new CException(TRUE);
}

void AFXAPI AfxAbort() {
    std::abort();
}

// CFileException::GetErrorMessage implementation
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

    if (pszMessage) {
        wcsncpy(lpszError, pszMessage, nMaxError - 1);
        lpszError[nMaxError - 1] = L'\0';
        return 1;
    }

    lpszError[0] = L'\0';
    return 0;
}
