// Define OPENMFC_APPCORE_IMPL since we link with appcore.cpp
// Define OPENMFC_FULL_IMPL to use full implementations instead of inline stubs
#define OPENMFC_APPCORE_IMPL
#define OPENMFC_FULL_IMPL
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
