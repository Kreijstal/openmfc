// Define OPENMFC_APPCORE_IMPL since we link with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cstdio>
#include <new>
#include <cwchar>

IMPLEMENT_DYNAMIC(CArchiveException, CException)

namespace {

void CopyExceptionText(wchar_t* out, UINT maxLen, const wchar_t* text) {
    if (out == nullptr || maxLen == 0) {
        return;
    }
    const wchar_t* src = text ? text : L"";
    wcsncpy(out, src, maxLen - 1);
    out[maxLen - 1] = L'\0';
}

const wchar_t* ArchiveCauseText(int cause) {
    switch (cause) {
    case CArchiveException::none: return L"No error";
    case CArchiveException::generic: return L"Archive error";
    case CArchiveException::readOnly: return L"Cannot write to read-only archive";
    case CArchiveException::endOfFile: return L"Unexpected end of file";
    case CArchiveException::writeOnly: return L"Cannot read from write-only archive";
    case CArchiveException::badIndex: return L"Invalid object index";
    case CArchiveException::badClass: return L"Invalid class found in archive";
    case CArchiveException::badSchema: return L"Schema mismatch in archive";
    case CArchiveException::badFormat: return L"Bad archive format";
    default: return L"Unknown archive error";
    }
}

} // namespace

class CInvalidArgException : public CException {
public:
    CInvalidArgException() : CException(TRUE) {}
};

class CNotSupportedException : public CException {
public:
    CNotSupportedException() : CException(TRUE) {}
};

class CResourceException : public CException {
public:
    CResourceException() : CException(TRUE) {}
};

class CUserException : public CException {
public:
    CUserException() : CException(TRUE) {}
};

namespace {
void EmitExceptionText(const wchar_t* text) {
    if (!text) return;
    fwprintf(stderr, L"%ls\n", text);
}
}

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
    throw new CInvalidArgException();
}

void AFXAPI AfxThrowNotSupportedException() {
    throw new CNotSupportedException();
}

void AFXAPI AfxThrowResourceException() {
    throw new CResourceException();
}

void AFXAPI AfxThrowUserException() {
    throw new CUserException();
}

void AFXAPI AfxAbort() {
    std::abort();
}

int CException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    if (pnHelpContext != nullptr) {
        *pnHelpContext = 0;
    }
    if (lpszError != nullptr && nMaxError > 0) {
        lpszError[0] = L'\0';
    }
    return 0;
}

void CException::Dump() const {
    const char* name = GetRuntimeClass() && GetRuntimeClass()->m_lpszClassName ? GetRuntimeClass()->m_lpszClassName : "CException";
    wchar_t out[128];
    size_t i = 0;
    for (; name[i] != '\0' && i + 1 < (sizeof(out)/sizeof(out[0])); ++i) {
        out[i] = static_cast<unsigned char>(name[i]);
    }
    out[i] = L'\0';
    fwprintf(stderr, L"CException::Dump class=%ls autoDelete=%d\n", out, m_bAutoDelete);
}

void CException::AssertValid() const {
    const CRuntimeClass* pClass = GetRuntimeClass();
    if (!pClass || !pClass->m_lpszClassName || pClass->m_lpszClassName[0] == '\0') {
        EmitExceptionText(L"CException::AssertValid invalid runtime class");
    }
    if (m_bAutoDelete != FALSE && m_bAutoDelete != TRUE) {
        EmitExceptionText(L"CException::AssertValid invalid m_bAutoDelete");
    }
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

void CFileException::Dump() const {
    wchar_t msg[1024];
    if (m_strFileName.IsEmpty()) {
        swprintf(msg, 1024, L"CFileException{cause=%d, os=%ld, autoDelete=%d}",
                 m_cause, m_lOsError, m_bAutoDelete);
    } else {
        swprintf(msg, 1024, L"CFileException{cause=%d, os=%ld, file=%ls, autoDelete=%d}",
                 m_cause, m_lOsError, static_cast<const wchar_t*>(m_strFileName), m_bAutoDelete);
    }
    fwprintf(stderr, L"%ls\n", msg);
}

void CFileException::AssertValid() const {
    CException::AssertValid();
}

CArchiveException::CArchiveException(int cause, const wchar_t* lpszArchiveName)
    : CException(TRUE), m_cause(cause), m_strFileName(lpszArchiveName ? lpszArchiveName : L"") {
}

int CArchiveException::GetErrorMessage(wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext) const {
    if (pnHelpContext != nullptr) {
        *pnHelpContext = 0;
    }
    if (lpszError == nullptr || nMaxError == 0) {
        return 0;
    }
    CopyExceptionText(lpszError, nMaxError, ArchiveCauseText(m_cause));
    return 1;
}

void CArchiveException::Dump() const {
    wchar_t msg[1024];
    if (m_strFileName.IsEmpty()) {
        swprintf(msg, 1024, L"CArchiveException{cause=%d, autoDelete=%d}", m_cause, m_bAutoDelete);
    } else {
        swprintf(msg, 1024, L"CArchiveException{cause=%d, file=%ls, autoDelete=%d}",
                 m_cause, static_cast<const wchar_t*>(m_strFileName), m_bAutoDelete);
    }
    fwprintf(stderr, L"%ls\n", msg);
}

void CArchiveException::AssertValid() const {
    CException::AssertValid();
}
