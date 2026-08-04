// CException — OpenMFC implementation.
// Sources: global_ctordtor_param.cpp, mfc_exceptions.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfcExceptionsSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ??0CException@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CException__QEAA_H_Z(void* p, int bAutoDelete) {
    return new (p) CException(bAutoDelete);
}
// Symbol: ?Dump@CException@@UEBAXXZ
extern "C" void MS_ABI impl__Dump_CException__UEBAXXZ(const CException* pThis) {
    if (pThis) {
        pThis->CException::Dump();
    }
}
// Symbol: ?AssertValid@CException@@UEBAXXZ
extern "C" void MS_ABI impl__AssertValid_CException__UEBAXXZ(const CException* pThis) {
    if (pThis) {
        pThis->CException::AssertValid();
    }
}
// Symbol: ?GetErrorMessage@CException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z(
    const CException* pThis, wchar_t* lpszError, UINT nMaxError, UINT* pnHelpContext
) {
    if (!pThis) {
        if (pnHelpContext) *pnHelpContext = 0;
        if (lpszError && nMaxError > 0) lpszError[0] = L'\0';
        return 0;
    }
    return pThis->CException::GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}
// Symbol: ?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ() {
    return &CException::classCException;
}
// Symbol: ?GetRuntimeClass@CException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CException__UEBAPEAUCRuntimeClass__XZ(const CException*) {
    return &CException::classCException;
}
// Symbol: ?Delete@CException@@QEAAXXZ
extern "C" void MS_ABI impl__Delete_CException__QEAAXXZ(CException* pThis) {
    if (pThis) pThis->Delete();
}
// Symbol: ?ReportError@CException@@UEAAHII@Z
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException* pThis, unsigned int type, unsigned int) {
    if (!pThis) return FALSE;
    wchar_t buffer[512] = {};
    pThis->GetErrorMessage(buffer, 512, nullptr);
    ::MessageBoxW(nullptr, buffer[0] ? buffer : L"An MFC exception occurred.", L"OpenMFC", type ? type : (MB_OK | MB_ICONERROR));
    return TRUE;
}
int CException::GetErrorMessage(wchar_t* lpszError, unsigned int nMaxError, unsigned int* pnHelpContext) const {
    if (pnHelpContext) {
        *pnHelpContext = 0;
    }
    if (!lpszError || nMaxError == 0) {
        return 0;
    }

    wchar_t className[64];
    CopyClassName(this, className, sizeof(className) / sizeof(className[0]));

    const wchar_t* message = nullptr;
    if (wcscmp(className, L"CMemoryException") == 0) {
        message = L"Out of memory.";
    } else if (wcscmp(className, L"CNotSupportedException_MfcExceptions") == 0) {
        message = L"Operation not supported.";
    } else if (wcscmp(className, L"CResourceException_MfcExceptions") == 0) {
        message = L"Resource failure.";
    } else if (wcscmp(className, L"CUserException_MfcExceptions") == 0) {
        message = L"User terminated the operation.";
    } else if (wcscmp(className, L"CInvalidArgException_MfcExceptions") == 0) {
        message = L"Invalid argument.";
    } else if (wcscmp(className, L"COleException") == 0) {
        message = L"OLE exception.";
    } else if (wcscmp(className, L"COleDispatchException") == 0) {
        message = L"OLE dispatch exception.";
    } else if (wcscmp(className, L"CFileException") == 0) {
        message = L"File exception.";
    } else if (wcscmp(className, L"CArchiveException") == 0) {
        message = L"Archive exception.";
    } else {
        message = L"MFC exception.";
    }

    CopyErrorText(lpszError, nMaxError, message);
    return 1;
}
void CException::Dump() const {
    wchar_t className[64];
    CopyClassName(this, className, sizeof(className) / sizeof(className[0]));
    wchar_t buffer[160];
    _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
               L"%ls[autoDelete=%d]\n", className, m_bAutoDelete);
    buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
    EmitDiagnosticText(buffer);
}
void CException::AssertValid() const {
    CRuntimeClass* pClass = GetRuntimeClass();
    if (!pClass || !pClass->m_lpszClassName || pClass->m_lpszClassName[0] == '\0') {
        EmitDiagnosticText(L"CException::AssertValid missing runtime class information\n");
    }
    if (m_bAutoDelete != FALSE && m_bAutoDelete != TRUE) {
        wchar_t buffer[96];
        _snwprintf(buffer, sizeof(buffer) / sizeof(buffer[0]),
                   L"CException::AssertValid unusual m_bAutoDelete=%d\n", m_bAutoDelete);
        buffer[(sizeof(buffer) / sizeof(buffer[0])) - 1] = L'\0';
        EmitDiagnosticText(buffer);
    }
}
